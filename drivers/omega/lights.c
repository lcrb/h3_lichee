#include "lights.h"
#include "pwm.h"
#include "sysfs_utils.h"

#include <linux/bug.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/gpio.h>

static pwm_t *_pwm_r = &pwms[0];
static pwm_t *_pwm_g = &pwms[1];
static pwm_t *_pwm_b = &pwms[2];
static pwm_t *_pwm_w = &pwms[3];
static pwm_t *_pwm_backlight = &pwms[4];

static struct kobject *_kobject;

#define RED_PIN 133
#define GREEN_PIN 134
#define BLUE_PIN 135
#define WHITE_PIN 132
#define BTN_PWM_PIN 129

void lights_init(void) {

    pwm_init(_pwm_r, "red", RED_PIN, 0, 0, 10);
    pwm_init(_pwm_g, "green", GREEN_PIN, 0, 0, 10);
    pwm_init(_pwm_b, "blue", BLUE_PIN, 0, 0, 10);
    pwm_init(_pwm_w, "white", WHITE_PIN, 0, 1, 10);

    // requested via SPI_GPIO_INIT in first stage of boot
    // TODO: improve
    gpio_free(BTN_PWM_PIN);
    pwm_init(_pwm_backlight, "backlight", BTN_PWM_PIN, 100, 0, 10);

    pwm_start(_pwm_r);
    pwm_start(_pwm_g);
    pwm_start(_pwm_b);
    pwm_start(_pwm_w);
    pwm_start(_pwm_backlight);
}

void lights_cleanup(void) {

    pwm_stop(_pwm_r);
    pwm_stop(_pwm_g);
    pwm_stop(_pwm_b);
    pwm_stop(_pwm_w);
    pwm_stop(_pwm_backlight);

    pwm_cleanup(_pwm_r);
    pwm_cleanup(_pwm_g);
    pwm_cleanup(_pwm_b);
    pwm_cleanup(_pwm_w);
    pwm_cleanup(_pwm_backlight);

    kobject_put(_kobject);
}

// private

SYSFS_GENERATE(red, _pwm_r->duty_cycle, 0, 100, 660, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(green, _pwm_g->duty_cycle, 0, 100, 660, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(blue, _pwm_b->duty_cycle, 0, 100, 660, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(white, _pwm_w->duty_cycle, 0, 100, 660, SYSFS_NULL, SYSFS_NULL);
SYSFS_GENERATE(backlight, _pwm_backlight->duty_cycle, 0, 100, 660, SYSFS_NULL, SYSFS_NULL);

void lights_setup_sysfs(void) {

    _kobject = kobject_create_and_add("beddi_pwm", kernel_kobj);

    BUG_ON(_kobject == NULL);

    SYSFS_CREATE_FILE(_kobject, red);
    SYSFS_CREATE_FILE(_kobject, green);
    SYSFS_CREATE_FILE(_kobject, blue);
    SYSFS_CREATE_FILE(_kobject, white);
    SYSFS_CREATE_FILE(_kobject, backlight);
}

int lights_test_step(void) {

    _pwm_r->duty_cycle = max(0, min(_pwm_r->duty_cycle + 10, 100));
    _pwm_g->duty_cycle = max(0, min(_pwm_g->duty_cycle + 10, 100));
    _pwm_b->duty_cycle = max(0, min(_pwm_b->duty_cycle + 10, 100));
    _pwm_w->duty_cycle = max(0, min(_pwm_w->duty_cycle + 10, 100));

    if (_pwm_r->duty_cycle == 100 \
            && _pwm_g->duty_cycle == 100 \
            && _pwm_b->duty_cycle == 100 \
            && _pwm_w->duty_cycle == 100) {

        return 1;
    }

    return 0;
}
