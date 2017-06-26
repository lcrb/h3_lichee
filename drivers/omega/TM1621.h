#ifndef __TM1621_H
#define __TM1621_H

#include "hardware_btn.h"

// Modes
#define READ_CMD                0b11000000
#define WRITE_CMD               0b10100000
#define READ_MODIFY_WRITE_CMD   0b10100000
#define COMMAND_CMD             0b10000000

// Commands
#define SYS_DIS                 0b0000100000000000  // 10000000000X
#define SYS_EN                  2050  // 10000000001X
#define LCD_OFF                 2052  // 10000000010X
#define LCD_ON                  2054  // 10000000011X
#define TIMER_DIS               2056  // 10000000100X
#define WDT_DIS                 2058  // 10000000101X
#define TIMER_EN                2060  // 10000000110X
#define WDT_EN                  2062  // 10000000111X
#define TONE_OFF                0b0000100000010000  // 10000001000X
#define TONE_ON                 0b0000100000010010  // 10000001001X
#define CLR_TIMER               0b0000100000011000  // 100000011XXX
#define CLR_WDT                 0b0000100000011100  // 1000000111XX
#define XTAL_32K                0b0000100000101000  // 100000101XXX
#define RC_256K                 2096  // 100000110XXX
#define EXT_256K                0b0000100000111000  // 100000111XXX
#define BIAS_1_2_2              2112  // 1000010abX0X
#define BIAS_1_2_3              2120  // 1000010abX0X
#define BIAS_1_2_4              2128  // 1000010abX0X
#define BIAS_1_3_2              2114  // 1000010abX1X
#define BIAS_1_3_3              2122  // 1000010abX1X
#define BIAS_1_3_4              2130  // 1000010abX1X
#define TONE_4K                 0b0000100010000000  // 100010XXXXXX
#define TONE_2K                 0b0000100011000000  // 100011XXXXXX
#define IRQ_DIS_B               0b0000100100000000  // 100100X0XXXX
#define IRQ_EN_B                0b0000100100010000  // 100100X1XXXX
#define F1                      0b0000100101000000  // 100101XX000X
#define F2                      0b0000100101000010  // 100101XX001X
#define F4                      0b0000100101000100  // 100101XX010X
#define F8                      0b0000100101000110  // 100101XX011X
#define F16                     0b0000100101001000  // 100101XX100X
#define F32                     0b0000100101001010  // 100101XX101X
#define F64                     0b0000100101001100  // 100101XX110X
#define F128                    0b0000100101001110  // 100101XX111X
#define TOPT                    0b0000100111000000  // 10011100000X
#define TNORMAL                 2502  // 10011100011X

#define BITS_PER_CMD            12

// Send Initial Commands
void initialize_lcd(void);
void initialize_gpio(void);

// Write 8 memory address, then write 4 data bits, repeated for specified conseuctive address
void write_data_consecutive(unsigned int addr_bits, unsigned int *data, unsigned int count);

// Send 9 Command bits in Command Mode
void send_command(unsigned int cmd_data);

#endif // __TM1621_H
