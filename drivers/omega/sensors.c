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

    ADC_ASSERT_HIGH();

    RESET_PIN(ADC_CSN_PIN);
    RESET_PIN(ADC_CLK_PIN);

    SET_PIN(ADC_DI_PIN); // start bit
    ADC_CLK_PULSE();
    SET_PIN(ADC_DI_PIN);  // sgl/diff always 1 for single-ended mode
    ADC_CLK_PULSE();

    if (channel == 1) {
        SET_PIN(ADC_DI_PIN); // odd/sign 0 for channel 0
    } else {
        RESET_PIN(ADC_DI_PIN); // odd/sign 0 for channel 0
    }

    ADC_CLK_PULSE();
    RESET_PIN(ADC_DI_PIN); // msbf == 0
    ADC_CLK_PULSE();

    ADC_CLK_PULSE(); // null bit

    val = 0;
    
    for (i = 0; i <= 11; i++) {
        ADC_CLK_PULSE();
        val |= (READ_PIN(ADC_DO_PIN) << i);
    }
    
    ADC_ASSERT_HIGH();
    __DELAY_US(1);

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
