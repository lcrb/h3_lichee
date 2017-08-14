#ifndef __HARDWARE_ADC_H
#define __HARDWARE_ADC_H

#include <linux/gpio.h>
#include <linux/delay.h>

// Signal Pins
#define ADC_CLK_PIN 96
#define ADC_DO_PIN 97
#define ADC_CSN_PIN 98
#define ADC_DI_PIN 99

#define __DELAY_MS(ms) mdelay(ms)
#define __DELAY_US(us) udelay(us)

#define SET_OUTPUT(GPIO_Pin) gpio_direction_output(GPIO_Pin, 1)
#define SET_INPUT(GPIO_Pin) gpio_direction_output(GPIO_Pin, 0)

#define SET_PIN(GPIO_Pin) gpio_set_value(GPIO_Pin, 1)
#define RESET_PIN(GPIO_Pin) gpio_set_value(GPIO_Pin, 0)

#define READ_PIN(GPIO_Pin) gpio_get_value(GPIO_Pin)

#define ADC_GPIO_INIT()    { \
    \
    gpio_request(ADC_CLK_PIN, "beddi_adc_clk_pin"); \
    gpio_request(ADC_DO_PIN, "beddi_adc_do_pin"); \
    gpio_request(ADC_CSN_PIN, "beddi_adc_csn_pin"); \
    gpio_request(ADC_DI_PIN, "beddi_adc_di_pin"); \
    ADC_DATA_IO_MODE(); \
    ADC_ASSERT_HIGH(); \
}

#define ADC_CLK_PULSE() { \
    \
    SET_PIN(ADC_CLK_PIN);\
    __DELAY_US(1);\
    RESET_PIN(ADC_CLK_PIN);\
    __DELAY_US(1);\
}

#define ADC_DATA_IO_MODE() { \
    SET_OUTPUT(ADC_CLK_PIN); \
    SET_OUTPUT(ADC_DI_PIN); \
    SET_OUTPUT(ADC_CSN_PIN); \
    SET_INPUT(ADC_DO_PIN); \
}

// Assert High
#define ADC_ASSERT_HIGH() { \
    SET_PIN(ADC_CLK_PIN); \
    SET_PIN(ADC_DI_PIN); \
    SET_PIN(ADC_CSN_PIN); \
}

#define ADC_GPIO_CLEANUP() { \
    ADC_DATA_IO_MODE(); \
    gpio_free(ADC_CLK_PIN); \
    gpio_free(ADC_CSN_PIN); \
    gpio_free(ADC_DI_PIN); \
    gpio_free(ADC_DO_PIN); \
}

#endif // __HARDWARE_ADC_H
