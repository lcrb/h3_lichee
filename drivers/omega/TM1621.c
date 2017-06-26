#include "TM1621.h"

// Send Initial Commands
void initialize_lcd(void) {

    // Initialize Pins
    initialize_gpio();

    // Initial Settings Commands
    send_command(LCD_OFF);
    send_command(WDT_DIS);
    send_command(TIMER_DIS);
    __DELAY_MS(50);
    send_command(SYS_EN);
    send_command(TNORMAL);
    send_command(RC_256K);
    send_command(BIAS_1_3_4);
    send_command(LCD_ON);
}

void initialize_gpio(void) {

    SPI_GPIO_INIT();
}

// Write 8 memory address, then write 4 data bits, repeated for specified conseuctive address
void write_data_consecutive(unsigned int addr_bits, unsigned int *data, unsigned int count) {
    int i;
    unsigned int j;

    // Combine with Write CMD bits
    addr_bits |= WRITE_CMD;

    // All Communication Lines Default High
    ASSERT_HIGH();

    // Pull CS_N Low
    SPI_CSN_LOW();
    __DELAY_US(4);

    for (i = 7; i > -1; i--) {
        SPI_WRN_LOW();
        SPI_DATA_CONDITION((addr_bits >> i) & 0x01);
        SPI_WRN_HIGH();
    }

    // Mystery Cycle
    SPI_WRN_LOW();
    SPI_WRN_HIGH();


    for (j = 0; j < count; j++) {
        for (i = 0; i < 4; i++) {
            SPI_WRN_LOW();
            SPI_DATA_CONDITION((data[j] >> i) & 0x01);
            SPI_WRN_HIGH();
        }
    }

    // All Communication Lines Default High
    ASSERT_HIGH();
}

// Send 9 Command bits in Command Mode
void send_command(unsigned int cmd_data) {
  int i;

  // All Communication Lines Default High
  ASSERT_HIGH();

  SPI_CSN_LOW();

  for (i = (BITS_PER_CMD - 1); i > -1; i--) {
    SPI_WRN_LOW();
    SPI_DATA_CONDITION((cmd_data >> i) & 0x01);
    __DELAY_US(1);
    SPI_WRN_HIGH();
  }

  // All Communication Lines Default High
  ASSERT_HIGH();
}
