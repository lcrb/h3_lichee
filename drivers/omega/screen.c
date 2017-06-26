#include "screen.h"
#include "GF90970.h"
#include "sysfs_utils.h"

#include <linux/interrupt.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/bug.h>
#include <linux/module.h>
#include <linux/kernel.h>

static struct kobject *_kobject;

static struct hrtimer _timer;
static struct hrtimer _secondary_timer;

static ktime_t _period;

enum hrtimer_restart boot_anim_timer_function(struct hrtimer *timer);
enum hrtimer_restart normal_timer_function(struct hrtimer *timer);
enum hrtimer_restart flash_timer_function(struct hrtimer *timer);
enum hrtimer_restart alarm_edit_timer_function(struct hrtimer *timer);

void flash_timer_init(void);
void volume_timer_init(void);

static void __mode_updated(void);
static void mode_updated(int cleanup);

static void timer_init(struct hrtimer *timer, enum hrtimer_restart (*tick)(struct hrtimer *timer), unsigned long period);
static void timer_cleanup(void);

static void screen_update_time(void);
static void screen_update_next_alarm_time(void);
static void screen_update_editing_alarm_time(void);
static void screen_update_wifi(void);
static void screen_update_nap_sleep(void);

typedef enum {
    MODE_BOOT,
    MODE_NORMAL,
    MODE_FLASH,
    MODE_VOLUME,
    MODE_ALARM_EDIT,
    MODE_INVALID,
} screen_mode_t;

static void (*_mode_init[MODE_INVALID])() = {
    NULL,
    NULL,
    flash_timer_init,
    volume_timer_init,
    NULL,
};

static enum hrtimer_restart (*_mode_table[MODE_INVALID])(struct hrtimer *timer) = {
    boot_anim_timer_function,
    normal_timer_function,
    flash_timer_function,
    flash_timer_function,
    alarm_edit_timer_function,
};

static unsigned long _mode_period[MODE_INVALID] = {
    200000000,
    1000000000,
    3000000000,
    3000000000,
    100000000,
};

static struct hrtimer * _mode_timer[MODE_INVALID] = {
    &_timer,
    &_timer,
    &_secondary_timer,
    &_secondary_timer,
    &_secondary_timer,
};

typedef struct {
    screen_mode_t mode;
    int hour;
    int minute;
    int military_time;
    int wifi_detected;
    int nap;
    int sleep;
    int next_alarm_hour;
    int next_alarm_minute;
    int flash_value;
    int volume;
    int audio_module;
    int editing_alarm_hour;
    int editing_alarm_minute;
} screen_sysfs_t;

static screen_sysfs_t _screen;

void screen_init_stage0(void) {
    _screen.next_alarm_hour = -1;
    _screen.next_alarm_minute = -1;

    GF90970_init();

    GF90970_set_boot_stage_0();
    GF90970_sync();
}

void screen_init(void) {

    GF90970_cleanup();

    // re-initialize BTN screen without cycling light
    GF90970_reinit();

    _screen.mode = MODE_BOOT;

    mode_updated(0);
}

void screen_cleanup(void) {

    timer_cleanup();
    GF90970_cleanup();

    kobject_put(_kobject);
}

// private

static void timer_init(struct hrtimer *timer, enum hrtimer_restart (*tick)(struct hrtimer *timer), unsigned long period) {

    _period = ktime_set(0, period);
    hrtimer_init (timer, CLOCK_REALTIME, HRTIMER_MODE_REL);
    timer->function = tick;
    hrtimer_start(timer, _period, HRTIMER_MODE_REL);
}

static void timer_cleanup(void) {

    if (hrtimer_active(&_timer) || hrtimer_is_queued(&_timer)) {
        hrtimer_cancel(&_timer);
    }

    if (hrtimer_active(&_secondary_timer) || hrtimer_is_queued(&_secondary_timer)) {
        hrtimer_cancel(&_secondary_timer);
    }
}

enum hrtimer_restart boot_anim_timer_function(struct hrtimer *timer) {

    GF90970_boot_anim_step();
    GF90970_sync();

    hrtimer_forward_now(timer, _period);

    return HRTIMER_RESTART;
}

enum hrtimer_restart normal_timer_function(struct hrtimer *timer) {

    screen_update_time();
    screen_update_next_alarm_time();
    screen_update_wifi();

    GF90970_sync();

    hrtimer_forward_now(timer, _period);

    return HRTIMER_RESTART;
}

enum hrtimer_restart flash_timer_function(struct hrtimer *timer) {

    _screen.mode = MODE_NORMAL;

    mode_updated(0);

    return HRTIMER_NORESTART;
}

enum hrtimer_restart alarm_edit_timer_function(struct hrtimer *timer) {

    screen_update_editing_alarm_time();

    GF90970_sync();

    hrtimer_forward_now(timer, _period);

    return HRTIMER_RESTART;
}

void flash_timer_init(void) {
    
    int i;
    int val;

    val = _screen.flash_value;

    for (i = 4; i >= 1; i--) {
        GF90970_set_digit(i, val % 10);
        val /= 10;

        if (val == 0) {
            break;
        }
    }

    GF90970_sync();
}

void volume_timer_init(void) {

    int i;
    int val;

    val = _screen.volume;

    for (i = 4; i >= 3; i--) {
        GF90970_set_digit(i, val % 10);
        val /= 10;

        if (val == 0) {
            break;
        }
    }

    GF90970_sync();
}

static void __mode_updated(void) {
    
    mode_updated(1);
}

static void mode_updated(int cleanup) {

    screen_mode_t mode;

    if (cleanup) {
        timer_cleanup();
    }

    GF90970_clear();
    GF90970_sync();

    screen_update_nap_sleep();

    mode = _screen.mode;

    WARN_ON(mode == MODE_INVALID);
    BUG_ON(_mode_table[mode] == NULL);

    if (_mode_init[mode]) {
        _mode_init[mode]();
    }

    timer_init(_mode_timer[mode], _mode_table[mode], _mode_period[mode]);
}

SYSFS_GENERATE(mode, _screen.mode, MODE_BOOT, MODE_INVALID, 660, __mode_updated, SYSFS_NULL);
SYSFS_GENERATE(hour, _screen.hour, 0, 23, 660, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(minute, _screen.minute, 0, 59, 660, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(military_time, _screen.military_time, 0, 1, 660, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(wifi, _screen.wifi_detected, 0, 1, 440, SYSFS_NULL, SYSFS_NULL); 
SYSFS_GENERATE(nap, _screen.nap, 0, 1, 440, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(sleep, _screen.sleep, 0, 1, 440, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(next_alarm_hour, _screen.next_alarm_hour, -1, 23, 440, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(next_alarm_minute, _screen.next_alarm_minute, -1, 59, 440, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(editing_alarm_hour, _screen.editing_alarm_hour, -1, 23, 440, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(editing_alarm_minute, _screen.editing_alarm_minute, -1, 59, 440, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(flash_value, _screen.flash_value, 0, 9999, 440, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(volume, _screen.volume, 0, 30, 660, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(audio_module, _screen.audio_module, 1, 15, 660, SYSFS_NULL, SYSFS_NULL);

void screen_setup_sysfs(void) {

    _kobject = kobject_create_and_add("beddi_btn", kernel_kobj);

    BUG_ON(_kobject == NULL);

    SYSFS_CREATE_FILE(_kobject, mode);
    SYSFS_CREATE_FILE(_kobject, hour);
    SYSFS_CREATE_FILE(_kobject, minute);
    SYSFS_CREATE_FILE(_kobject, military_time);
    SYSFS_CREATE_FILE(_kobject, wifi);
    SYSFS_CREATE_FILE(_kobject, nap);
    SYSFS_CREATE_FILE(_kobject, sleep);
    SYSFS_CREATE_FILE(_kobject, next_alarm_hour);
    SYSFS_CREATE_FILE(_kobject, next_alarm_minute);
    SYSFS_CREATE_FILE(_kobject, editing_alarm_hour);
    SYSFS_CREATE_FILE(_kobject, editing_alarm_minute);
    SYSFS_CREATE_FILE(_kobject, flash_value);
    SYSFS_CREATE_FILE(_kobject, volume);
    SYSFS_CREATE_FILE(_kobject, audio_module);
}

static void screen_update_time(void) {
    static int show_separator;
    int h, m, military;

    h = _screen.hour;
    m = _screen.minute;
    military = _screen.military_time;

    if (military) {
        GF90970_am_off();
        GF90970_pm_off();
    } else {
        if (h < 12) {
            GF90970_am_on();
            GF90970_pm_off();
        } else {
            GF90970_am_off();
            GF90970_pm_on();
        }
    }

    if (!military) {
         if (h > 12) {
            h -= 12;
        } else if (h == 0) {
            h = 12;
        }
    }

    if (h > 9 || military) {
        GF90970_set_digit(1, h / 10);
    } else {
        GF90970_clear_digit(1);
    }

    GF90970_set_digit(2, h % 10);

    GF90970_set_digit(3, m / 10);
    GF90970_set_digit(4, m % 10);

    if (show_separator) {
        GF90970_main_time_div_on();
    } else {
        GF90970_main_time_div_off();
    }

    show_separator = !show_separator;
}

static void screen_update_next_alarm_time(void) {
    
    int h, m;

    h = _screen.next_alarm_hour;
    m = _screen.next_alarm_minute;

    if (h == -1 || m == -1) {
        GF90970_clear_digit(8);
        GF90970_clear_digit(9);
        GF90970_clear_digit(10);
        GF90970_clear_digit(11);
        GF90970_alarm_time_div_off();
        GF90970_alarm_set_off();
        
        return;
    }
    
    GF90970_set_digit(8, h / 10);
    GF90970_set_digit(9, h % 10);

    GF90970_set_digit(10, m / 10);
    GF90970_set_digit(11, m % 10);

    GF90970_alarm_time_div_on();
    GF90970_alarm_set_on();
}

static void screen_update_editing_alarm_time(void) {

    static int count;
    static int alarm_segment_on;
    
    int h, m;

    h = _screen.editing_alarm_hour;
    m = _screen.editing_alarm_minute;

    count++;

    if (count == 5) {
        count = 0;
        alarm_segment_on = 1 - alarm_segment_on;
    }
    
    GF90970_set_digit(8, h / 10);
    GF90970_set_digit(9, h % 10);

    GF90970_set_digit(10, m / 10);
    GF90970_set_digit(11, m % 10);

    GF90970_alarm_time_div_on();

    if (alarm_segment_on) {
        GF90970_alarm_set_on();
    } else {
        GF90970_alarm_set_off();
    }
}

static void screen_update_wifi(void) {

    if (_screen.wifi_detected) {
        GF90970_wifi_detected_on();
    } else {
        GF90970_wifi_detected_off();
    }
}

static void screen_update_nap_sleep(void) {

    if (_screen.nap) {
        GF90970_nap_on();
    } else {
        GF90970_nap_off();
    }

    if (_screen.sleep) {
        GF90970_snooze_on();
    } else {
        GF90970_snooze_off();
    }
}
