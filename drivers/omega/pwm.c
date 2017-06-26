#include "pwm.h"

#include <linux/string.h>

pwm_t pwms[5];

static pwm_t *get_pwm_from_timer(struct hrtimer *timer);
static enum hrtimer_restart timer_function(struct hrtimer * timer);

void pwm_init(pwm_t *pwm, const char *name, int pin, int initial_duty, int inverted, unsigned long period) {
    
    memset(pwm, 0, sizeof(pwm_t));

    pwm->pin = pin;
    pwm->duty_cycle = initial_duty;
    pwm->period = period;
    pwm->inverted = inverted;

    gpio_request(pin, name);
    gpio_direction_output(pin, pwm->inverted ? 1 : 0);
}

void pwm_start(pwm_t *pwm) {

    pwm->_period = ktime_set(0, 0); //seconds,nanoseconds
    hrtimer_init(&pwm->timer, CLOCK_REALTIME, HRTIMER_MODE_REL);
    pwm->timer.function = timer_function;
    hrtimer_start(&pwm->timer, pwm->_period, HRTIMER_MODE_REL);
}

void pwm_stop(pwm_t *pwm) {
    
    hrtimer_cancel(&pwm->timer);
}

void pwm_cleanup(pwm_t *pwm) {

    gpio_direction_output(pwm->pin, pwm->inverted ? 1 : 0);
    gpio_free(pwm->pin);
}

// private

static pwm_t *get_pwm_from_timer(struct hrtimer *timer) {
    int i;

    for (i = 0; i < sizeof(pwms) / sizeof(pwm_t); i++) {
        if (&pwms[i].timer == timer) {
            return &pwms[i];
        }
    }

    return NULL;
}

static enum hrtimer_restart timer_function(struct hrtimer * timer) {
    unsigned long time;

    pwm_t *pwm = (pwm_t *)get_pwm_from_timer(timer);

    BUG_ON(pwm == NULL);
    
    if (pwm->duty_cycle == 0) {
        gpio_set_value(pwm->pin, pwm->inverted ? 1 : 0);
        hrtimer_forward_now(timer, pwm->_period);
        return HRTIMER_RESTART;

    } else if(pwm->duty_cycle == 100) {
        gpio_set_value(pwm->pin, pwm->inverted ? 0 : 1);
        hrtimer_forward_now(timer, pwm->_period);
        return HRTIMER_RESTART;
    }

    gpio_set_value(pwm->pin, pwm->inverted ? pwm->on : !pwm->on);

    if (pwm->on) {
        time = pwm->period * (100 - pwm->duty_cycle);
        pwm->on = 0;
    } else {
        time = (pwm->period * pwm->duty_cycle);
        pwm->on = 1;
    }

    time *= 10000;

    pwm->_period = ktime_set(0, time); //seconds,nanoseconds

    hrtimer_forward_now(timer, pwm->_period);

    return HRTIMER_RESTART;
}
