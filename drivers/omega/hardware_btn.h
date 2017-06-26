#ifndef __HARDWARE_BTN_H
#define __HARDWARE_BTN_H

#include <linux/gpio.h>
#include <linux/delay.h>

// Signal Pins
#define WR_N_PIN 130
#define CS_N_PIN 128
#define DATA_PIN 131
#define BRIGHTNESS_PIN 129

#define __DELAY_MS(ms) mdelay(ms)
#define __DELAY_US(us) udelay(us)

#define SET_OUTPUT(GPIO_Pin) gpio_direction_output(GPIO_Pin, 1)

#define SET_PIN(GPIO_Pin) gpio_set_value(GPIO_Pin, 1)
#define RESET_PIN(GPIO_Pin) gpio_set_value(GPIO_Pin, 0)

#define SPI_GPIO_INIT()    { \
    gpio_request(BRIGHTNESS_PIN, "beddi_brightness_pin"); \
    SET_OUTPUT(BRIGHTNESS_PIN); \
    \
    gpio_request(WR_N_PIN, "beddi_wr_n_pin"); \
    gpio_request(CS_N_PIN, "beddi_cs_n_pin"); \
    gpio_request(DATA_PIN, "beddi_data_pin"); \
    SPI_DATA_OUTPUT_MODE(); \
    ASSERT_HIGH(); \
}

#define SPI_WRN_LOW() RESET_PIN(WR_N_PIN)
#define SPI_WRN_HIGH() SET_PIN(WR_N_PIN)

#define SPI_CSN_LOW() RESET_PIN(CS_N_PIN)
#define SPI_CSN_HIGH() SET_PIN(CS_N_PIN)

#define SPI_DATA_LOW() RESET_PIN(DATA_PIN)
#define SPI_DATA_HIGH() SET_PIN(DATA_PIN)

#define SPI_DATA_CONDITION(data) if (data) { SPI_DATA_HIGH(); } else SPI_DATA_LOW();


#define SPI_DATA_OUTPUT_MODE() { \
    SET_OUTPUT(WR_N_PIN); \
    SET_OUTPUT(CS_N_PIN); \
    SET_OUTPUT(DATA_PIN); \
}

// Assert High
#define ASSERT_HIGH() { \
    SPI_WRN_HIGH(); \
    SPI_CSN_HIGH(); \
    SPI_DATA_HIGH(); \
}

#define GPIO_CLEANUP() { \
    SPI_DATA_OUTPUT_MODE(); \
    gpio_free(WR_N_PIN); \
    gpio_free(CS_N_PIN); \
    gpio_free(DATA_PIN); \
}

#endif // __HARDWARE_BTN_H
