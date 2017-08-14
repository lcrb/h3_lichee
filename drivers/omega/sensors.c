#include "sensors.h"
#include "hardware_adc.h"
#include "sysfs_utils.h"

// TODO: see if this is needed and if so, give it a home
#define AUDIO_MUTE_PIN 16

static struct kobject *_sensor_kobject;

static int sensor_poll_channel(int channel);

static ssize_t __used temperature_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", sensor_poll_channel(0));
}

static ssize_t __used light_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", sensor_poll_channel(1));
}

static struct kobj_attribute temperature_attribute =__ATTR(temperature, 440, temperature_show, SYSFS_NULL);
static struct kobj_attribute light_attribute =__ATTR(light, 440, light_show, SYSFS_NULL);

static int sensor_poll_channel(int channel) {
    int val;
    int i;

    SET_PIN(ADC_CSN_PIN);
    RESET_PIN(ADC_CLK_PIN);
    RESET_PIN(ADC_DI_PIN);

    __DELAY_US(1);

    RESET_PIN(ADC_CSN_PIN); // begin poll

    SET_PIN(ADC_DI_PIN); // start bit
    __DELAY_US(1);

    // clock pulse
    SET_PIN(ADC_CLK_PIN);
    __DELAY_US(1);
    RESET_PIN(ADC_CLK_PIN);

    SET_PIN(ADC_DI_PIN); // sgl/diff
    __DELAY_US(1);

    // clock pulse
    SET_PIN(ADC_CLK_PIN);
    __DELAY_US(1);
    RESET_PIN(ADC_CLK_PIN);

    if (channel) { // channel 0/1
        SET_PIN(ADC_DI_PIN);
    } else {
        RESET_PIN(ADC_DI_PIN);
    }

    __DELAY_US(1);

    // clock pulse
    SET_PIN(ADC_CLK_PIN);
    __DELAY_US(1);
    RESET_PIN(ADC_CLK_PIN);

    SET_PIN(ADC_DI_PIN); // msb first
    __DELAY_US(1);

    // clock pulse
    SET_PIN(ADC_CLK_PIN);
    __DELAY_US(1);
    RESET_PIN(ADC_CLK_PIN);

    __DELAY_US(1);

    // clock pulse for null bit
    SET_PIN(ADC_CLK_PIN);
    __DELAY_US(1);
    RESET_PIN(ADC_CLK_PIN);

    val = 0;

    for (i = 11; i >= 0; i--) {
        SET_PIN(ADC_CLK_PIN);
        __DELAY_US(1);
        val |= (READ_PIN(ADC_DO_PIN) << i);
        RESET_PIN(ADC_CLK_PIN);
        __DELAY_US(1);
    }

    SET_PIN(ADC_CSN_PIN);

    return val;
}

void sensor_setup_sysfs(void) {
    int e;

    _sensor_kobject = kobject_create_and_add("beddi_sensors", kernel_kobj);

    BUG_ON(_sensor_kobject == NULL);

    e = sysfs_create_file(_sensor_kobject, &temperature_attribute);
    BUG_ON(e != 0);

    e = sysfs_create_file(_sensor_kobject, &light_attribute);
    BUG_ON(e != 0);
}

void sensor_init(void) {

    ADC_GPIO_INIT();
}

void sensor_cleanup(void) {

    ADC_GPIO_CLEANUP();
}
