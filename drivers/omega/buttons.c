#include "buttons.h"
#include "sysfs_utils.h"
#include "GF90970.h"

#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/module.h> 

#define VOLUME_DOWN_PIN 356
#define VOLUME_UP_PIN 354
#define SNOOZE_PIN 352

#define SMART_1_PIN 199
#define ALARM_PIN 201
#define PLAY_PAUSE_PIN 355

static int reset_ap;
static int reset_presets;
static int _test_mode;

static struct kobject *_kobject;

void clear_ap_indicator(void) {
    
    GF90970_clear_digit(8);
    GF90970_clear_digit(9);
    GF90970_clear_digit(10);
    GF90970_clear_digit(11);

    reset_ap = 0;
}

void clear_presets_indicator(void) {
    
    GF90970_clear_digit(5);
    GF90970_clear_digit(6);

    reset_presets = 0;
}

SYSFS_GENERATE(ap, reset_ap, 0, 1, 440, SYSFS_NULL, clear_ap_indicator);
SYSFS_GENERATE(presets, reset_presets, 0, 1, 440, SYSFS_NULL, clear_presets_indicator);
SYSFS_GENERATE(test_mode, _test_mode, 0, 1, 440, SYSFS_NULL, SYSFS_NULL);

void buttons_pullup(void) {
    unsigned int regval;
    unsigned int *regs;

    // regs for volume up / snooze buttons
    regs = (unsigned int *)ioremap(0x01C20800 + 0xF4, 4);

    regval = ioread32(regs);
    regval &= ~(3 << 14);
    regval |= (1 << 14); // PG 7 pull up
    regval &= ~(3 << 18);
    regval |= (1 << 18); // PG 9 pull up
    iowrite32(regval, regs);
    regval = ioread32(regs);

    iounmap(regs);

    regs = (unsigned int *)ioremap(0x01C20800 + 0x1C, 4);

    regval = ioread32(regs);
    regval &= ~(3 << 0);
    regval |= (1 << 0); // PL 0 pull up
    regval &= ~(3 << 4);
    regval |= (1 << 4); // PL 2 pull up
    regval &= ~(3 << 6);
    regval |= (1 << 6); // PL 3 pull up
    regval &= ~(3 << 8);
    regval |= (1 << 8); // PL 4 pull up
    iowrite32(regval, regs);
    regval = ioread32(regs);

    iounmap(regs);
}

void buttons_setup_sysfs(void) {

    _kobject = kobject_create_and_add("beddi_reset", kernel_kobj);

    BUG_ON(_kobject == NULL);

    SYSFS_CREATE_FILE(_kobject, ap);
    SYSFS_CREATE_FILE(_kobject, presets);
    SYSFS_CREATE_FILE(_kobject, test_mode);
}

void buttons_run(void) {

    gpio_request(VOLUME_DOWN_PIN, "volume_down_reset_pin");
    gpio_request(VOLUME_UP_PIN, "volume_up_reset_pin");
    gpio_request(SNOOZE_PIN, "preset_reset_pin");

    gpio_direction_input(VOLUME_DOWN_PIN);
    gpio_direction_input(VOLUME_UP_PIN);
    gpio_direction_input(SNOOZE_PIN);

    mdelay(10);
    buttons_pullup();
    mdelay(100);

    reset_ap = (gpio_get_value(VOLUME_DOWN_PIN) == 0) && (gpio_get_value(VOLUME_UP_PIN) == 0);
    reset_presets = gpio_get_value(SNOOZE_PIN) == 0;

    if (reset_ap) {
        GF90970_ap_reset();
    }

    if (reset_presets) {
        GF90970_presets_reset();
    }

    gpio_free(VOLUME_UP_PIN);
    gpio_free(VOLUME_DOWN_PIN);
    gpio_free(SNOOZE_PIN);
} 

void buttons_cleanup(void) {

    kobject_put(_kobject);
}

int buttons_check_test_mode(void) {

    int ret = 0;
    
    gpio_request(SMART_1_PIN, "smart_1_pin");
    gpio_request(ALARM_PIN, "smart_3_pin");
    gpio_request(PLAY_PAUSE_PIN, "play_pause_pin");

    gpio_direction_input(SMART_1_PIN);
    gpio_direction_input(ALARM_PIN);
    gpio_direction_input(PLAY_PAUSE_PIN);

    mdelay(10);
    buttons_pullup();
    mdelay(100);

    if (gpio_get_value(SMART_1_PIN) == 0 && gpio_get_value(ALARM_PIN) == 0 && gpio_get_value(PLAY_PAUSE_PIN) == 0) {
        _test_mode = 1;
        ret = 1;
    }

    gpio_free(SMART_1_PIN);
    gpio_free(ALARM_PIN);
    gpio_free(PLAY_PAUSE_PIN);

    return ret;
}
