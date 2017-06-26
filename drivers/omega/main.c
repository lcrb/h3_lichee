#include "lights.h"
#include "screen.h"
#include "buttons.h"
#include "test.h"

#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>

static struct hrtimer _timer;

static enum hrtimer_restart boot_offset_timer_handler(struct hrtimer *timer) {

    // check buttons for reset flags
    if (buttons_check_test_mode()) {
        test_init();
    } else {
        screen_init();
        buttons_run();
        lights_init();
    }

    return HRTIMER_NORESTART;
}

static void boot_offset_timer_init(void) {

    hrtimer_init(&_timer, CLOCK_REALTIME, HRTIMER_MODE_REL);
    _timer.function = boot_offset_timer_handler;

    hrtimer_start(&_timer, ktime_set(3, 0), HRTIMER_MODE_REL);
}

int __init beddi_init(void) {

    screen_setup_sysfs();
    lights_setup_sysfs();
    buttons_setup_sysfs();

    screen_init_stage0();
    boot_offset_timer_init();

    return 0;
}

void __exit beddi_exit(void) {

    buttons_cleanup();
    screen_cleanup();
    lights_cleanup();
}

MODULE_LICENSE("GPL");

module_init(beddi_init);
module_exit(beddi_exit);
