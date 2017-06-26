#ifndef __PWM_H
#define __PWM_H

#include <linux/interrupt.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/gpio.h>

typedef struct {
    int duty_cycle;
    unsigned long period;
    int pin;
    int inverted;

    int on;
    struct hrtimer timer;
    ktime_t _period;
} pwm_t;

void pwm_init(pwm_t *pwm, const char *name, int pin, int initial_duty, int inverted, unsigned long period);
void pwm_start(pwm_t *pwm);
void pwm_stop(pwm_t *pwm);
void pwm_cleanup(pwm_t *pwm);

extern pwm_t pwms[5];

#endif // __PWM_H
