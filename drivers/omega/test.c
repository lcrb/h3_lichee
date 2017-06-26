#include "test.h"
#include "lights.h"

#include "GF90970.h"

#include <linux/interrupt.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>

static void test_screen(void);
static void test_lights(void);

static void screen_test_timer_init(void);
static enum hrtimer_restart screen_test_timer_handler(struct hrtimer *timer);

static void lights_test_timer_init(void);
static enum hrtimer_restart lights_test_timer_handler(struct hrtimer *timer);

static struct kobject *_kobject;

static ktime_t _period;
static struct hrtimer _timer;
static struct hrtimer _lights_timer;

static int _flash_counter;
static int _flash_on = 1;

static enum hrtimer_restart screen_test_timer_handler(struct hrtimer *timer) {

    if (_flash_counter >= 6) {

        // the else case is run last, so only need to disable temp units
        GF90970_temp_units_off();
        GF90970_sync();

        return HRTIMER_NORESTART;
    }

    if (_flash_on) {
        GF90970_number_8(1);
        GF90970_number_8(2);
        GF90970_number_8(3);
        GF90970_number_8(4);

        GF90970_number_8(5);
        GF90970_number_8(6);

        GF90970_number_8(8);
        GF90970_number_8(9);
        GF90970_number_8(10);
        GF90970_number_8(11);

        GF90970_am_on();
        GF90970_pm_on();
        GF90970_temp_units_c();
        GF90970_temp_over_ninety_nine_on();
        GF90970_bt_detected_on();
        GF90970_main_time_div_on();
        GF90970_alarm_time_div_on();
        GF90970_alarm_set_on();
        GF90970_snooze_on();
        GF90970_nap_on();
        GF90970_wifi_detected_on();
    } else {
        GF90970_clear_digit(1);
        GF90970_clear_digit(2);
        GF90970_clear_digit(3);
        GF90970_clear_digit(4);

        GF90970_clear_digit(5);
        GF90970_clear_digit(6);

        GF90970_clear_digit(8);
        GF90970_clear_digit(9);
        GF90970_clear_digit(10);
        GF90970_clear_digit(11);

        GF90970_am_off();
        GF90970_pm_off();
        GF90970_temp_units_f();
        GF90970_temp_over_ninety_nine_off();
        GF90970_bt_detected_off();
        GF90970_main_time_div_off();
        GF90970_alarm_time_div_off();
        GF90970_alarm_set_off();
        GF90970_snooze_off();
        GF90970_nap_off();
        GF90970_wifi_detected_off();
    }

    GF90970_sync();

    _flash_on = 1 - _flash_on;
    _flash_counter++;

    hrtimer_forward_now(timer, _period);

    return HRTIMER_RESTART;
}

static void screen_test_timer_init(void) {

    hrtimer_init(&_timer, CLOCK_REALTIME, HRTIMER_MODE_REL);
    _timer.function = screen_test_timer_handler;

    hrtimer_start(&_timer, _period, HRTIMER_MODE_REL);
}

static enum hrtimer_restart lights_test_timer_handler(struct hrtimer *timer) {

    int ret;
    
    ret = lights_test_step();

    if (ret == 1) {
        return HRTIMER_NORESTART;
    }

    hrtimer_forward_now(timer, _period);

    return HRTIMER_RESTART;
}

static void lights_test_timer_init(void) {

    hrtimer_init(&_lights_timer, CLOCK_REALTIME, HRTIMER_MODE_REL);
    _lights_timer.function = lights_test_timer_handler;

    hrtimer_start(&_lights_timer, _period, HRTIMER_MODE_REL);
}

void test_init(void) {

    _period = ktime_set(1, 0);

    test_screen();
    test_lights();
}

static void test_screen(void) {

    GF90970_cleanup();
    GF90970_reinit();

    GF90970_sync();

    screen_test_timer_init();
}

static void test_lights(void) {

    lights_init();

    lights_test_timer_init();
}
