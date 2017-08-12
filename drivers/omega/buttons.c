#include "buttons.h"
#include "sysfs_utils.h"
#include "GF90970.h"

#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/module.h> 

#define SMART_1_PIN 199
#define SMART_2_PIN 200
#define ALARM_PIN 201

#define VOLUME_DOWN_PIN 356
#define VOLUME_UP_PIN 354
#define SNOOZE_PIN 352
#define PLAY_PAUSE_PIN 355

static int reset_ap;
static int reset_presets;
static int _test_mode;

static struct kobject *_kobject;
static struct kobject *_button_kobject;

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

static ssize_t __used smart_1_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", gpio_get_value(SMART_1_PIN));
}

static ssize_t __used smart_2_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", gpio_get_value(SMART_2_PIN));
}

static ssize_t __used alarm_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", gpio_get_value(ALARM_PIN));
}

static ssize_t __used play_pause_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", gpio_get_value(PLAY_PAUSE_PIN));
}

static ssize_t __used vol_up_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", gpio_get_value(VOLUME_UP_PIN));
}

static ssize_t __used vol_down_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", gpio_get_value(VOLUME_DOWN_PIN));
}

static ssize_t __used snooze_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", gpio_get_value(SNOOZE_PIN));
}

static struct kobj_attribute smart_1_attribute =__ATTR(smart_1, 440, smart_1_show, SYSFS_NULL);
static struct kobj_attribute smart_2_attribute =__ATTR(smart_2, 440, smart_2_show, SYSFS_NULL);
static struct kobj_attribute alarm_attribute =__ATTR(alarm, 440, alarm_show, SYSFS_NULL);
static struct kobj_attribute play_pause_attribute =__ATTR(play_pause, 440, play_pause_show, SYSFS_NULL);
static struct kobj_attribute vol_up_attribute =__ATTR(vol_up, 440, vol_up_show, SYSFS_NULL);
static struct kobj_attribute vol_down_attribute =__ATTR(vol_down, 440, vol_down_show, SYSFS_NULL);
static struct kobj_attribute snooze_attribute =__ATTR(snooze, 440, snooze_show, SYSFS_NULL);

void buttons_pullup(void) {
    unsigned int regval;
    unsigned int *regs;

    // regs for volume up / snooze buttons
    regs = (unsigned int *)ioremap(0x01C20800 + 0xF4, 4);

    regval = ioread32(regs);
    regval &= ~(3 << 14);
    regval |= (1 << 14); // PG 7 pull up
    regval &= ~(3 << 16);
    regval |= (1 << 16); // PG 8 pull up
    regval &= ~(3 << 18);
    regval |= (1 << 18); // PG 9 pull up
    iowrite32(regval, regs);
    regval = ioread32(regs);

    iounmap(regs);

    regs = (unsigned int *)ioremap(0x01F02C00 + 0x1C, 4);

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

    int e;

    _kobject = kobject_create_and_add("beddi_reset", kernel_kobj);

    BUG_ON(_kobject == NULL);

    SYSFS_CREATE_FILE(_kobject, ap);
    SYSFS_CREATE_FILE(_kobject, presets);
    SYSFS_CREATE_FILE(_kobject, test_mode);

    _button_kobject = kobject_create_and_add("beddi_buttons", kernel_kobj);

    BUG_ON(_button_kobject == NULL);

    e = sysfs_create_file(_button_kobject, &smart_1_attribute);
    BUG_ON(e != 0);

    e = sysfs_create_file(_button_kobject, &smart_2_attribute);
    BUG_ON(e != 0);

    e = sysfs_create_file(_button_kobject, &alarm_attribute);
    BUG_ON(e != 0);

    e = sysfs_create_file(_button_kobject, &play_pause_attribute);
    BUG_ON(e != 0);

    e = sysfs_create_file(_button_kobject, &vol_up_attribute);
    BUG_ON(e != 0);

    e = sysfs_create_file(_button_kobject, &vol_down_attribute);
    BUG_ON(e != 0);

    e = sysfs_create_file(_button_kobject, &snooze_attribute);
    BUG_ON(e != 0);
}

void buttons_run(void) {

    gpio_request(VOLUME_DOWN_PIN, "volume_down_pin");
    gpio_request(VOLUME_UP_PIN, "volume_up_pin");
    gpio_request(SNOOZE_PIN, "snooze_pin");
    gpio_request(PLAY_PAUSE_PIN, "play_pause_pin");
    gpio_request(ALARM_PIN, "alarm_pin");
    gpio_request(SMART_1_PIN, "smart_1_pin");
    gpio_request(SMART_2_PIN, "smart_2_pin");

    gpio_direction_input(VOLUME_DOWN_PIN);
    gpio_direction_input(VOLUME_UP_PIN);
    gpio_direction_input(SNOOZE_PIN);
    gpio_direction_input(PLAY_PAUSE_PIN);
    gpio_direction_input(ALARM_PIN);
    gpio_direction_input(SMART_1_PIN);
    gpio_direction_input(SMART_2_PIN);

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
