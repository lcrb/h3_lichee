#include "GF90970.h"
#include "TM1621.h"

#include <stdbool.h>
#include <linux/string.h>

// Big Ole Array
static int SEG_COM_GRID[21][4];

// Digit 1
static int A1_LCD[2] = {1, 3};
static int B1_LCD[2] = {1, 2};
static int C1_LCD[2] = {1, 1};
static int D1_LCD[2] = {1, 0};
static int E1_LCD[2] = {0, 0};
static int G1_LCD[2] = {0, 1};
static int F1_LCD[2] = {0, 2};

// AM or PM
static int AM_LCD[2] = {0, 3};
static int PM_LCD[2] = {2, 0};

// Digit 2
static int A2_LCD[2] = {3, 3};
static int B2_LCD[2] = {3, 2};
static int C2_LCD[2] = {3, 1};
static int D2_LCD[2] = {3, 0};
static int E2_LCD[2] = {2, 1};
static int F2_LCD[2] = {2, 3};
static int G2_LCD[2] = {2, 2};

// Digit 3
static int A3_LCD[2] = {5, 3};
static int B3_LCD[2] = {5, 2};
static int C3_LCD[2] = {5, 1};
static int D3_LCD[2] = {5, 0};
static int E3_LCD[2] = {4, 1};
static int G3_LCD[2] = {4, 2};
static int F3_LCD[2] = {4, 3};

// Time Divider
static int COL1_LCD[2] = {4, 0};

// Digit 4
static int A4_LCD[2] = {7, 3};
static int B4_LCD[2] = {7, 2};
static int C4_LCD[2] = {7, 1};
static int D4_LCD[2] = {7, 0};
static int E4_LCD[2] = {6, 1};
static int G4_LCD[2] = {6, 2};
static int F4_LCD[2] = {6, 3};

// Digit 5
static int A5_LCD[2] = {9, 3};
static int B5_LCD[2] = {9, 2};
static int C5_LCD[2] = {9, 1};
static int D5_LCD[2] = {9, 0};
static int E5_LCD[2] = {8, 1};
static int G5_LCD[2] = {8, 2};
static int F5_LCD[2] = {8, 3};

// Pre 5,6
static int T3_LCD[2] = {8, 0};

// Digit 6
static int A6_LCD[2] = {11, 3};
static int B6_LCD[2] = {11, 2};
static int C6_LCD[2] = {11, 1};
static int D6_LCD[2] = {11, 0};
static int E6_LCD[2] = {10, 1};
static int G6_LCD[2] = {10, 2};
static int F6_LCD[2] = {10, 3};

// Nap
static int T5_LCD[2] = {10, 0};

// Degrees Symbol
static int T4_LCD[2] = {12, 3};

// Temperature Units
static int D7_LCD[2] = {12, 1};  // C.
static int G7_LCD[2] = {12, 2};  // F.

// Snooze ZZZ
static int T6_LCD[2] = {12, 0};

// Digit 8
static int A8_LCD[2] = {19, 0};
static int B8_LCD[2] = {19, 1};
static int C8_LCD[2] = {19, 2};
static int D8_LCD[2] = {19, 3};
static int E8_LCD[2] = {20, 3};
static int G8_LCD[2] = {20, 2};
static int F8_LCD[2] = {20, 1};

// Alarm Clock
static int T7_LCD[2] = {20, 0};

// Digit 9
static int A9_LCD[2] = {17, 0};
static int B9_LCD[2] = {17, 1};
static int C9_LCD[2] = {17, 2};
static int D9_LCD[2] = {17, 3};
static int E9_LCD[2] = {18, 3};
static int G9_LCD[2] = {18, 2};
static int F9_LCD[2] = {18, 1};

// Bluetooth
static int T8_LCD[2] = {18, 0};

// Digit 10
static int A10_LCD[2] = {15, 0};
static int B10_LCD[2] = {15, 1};
static int C10_LCD[2] = {15, 2};
static int D10_LCD[2] = {15, 3};
static int E10_LCD[2] = {16, 3};
static int G10_LCD[2] = {16, 2};
static int F10_LCD[2] = {16, 1};

// Timer 2 (Digits 8 through 11) Column Divider
static int COL2_LCD[2] = {16, 0};

// Digit 11
static int A11_LCD[2] = {13, 0};
static int B11_LCD[2] = {13, 1};
static int C11_LCD[2] = {13, 2};
static int D11_LCD[2] = {13, 3};
static int E11_LCD[2] = {14, 3};
static int G11_LCD[2] = {14, 2};
static int F11_LCD[2] = {14, 1};

// Wifi
static int T9_LCD[2] = {14, 0};

void GF90970_init(void) {
    initialize_lcd();
    GF90970_sync();
}

void GF90970_reinit(void) {

    initialize_gpio();
}

void GF90970_cleanup(void) {
    GPIO_CLEANUP();
    memset(SEG_COM_GRID, 0, sizeof(SEG_COM_GRID));
}

void GF90970_clear(void) {

    memset(SEG_COM_GRID, 0, sizeof(SEG_COM_GRID));
}

// AM on
void GF90970_am_on(void) {
    SEG_COM_GRID[AM_LCD[0]][AM_LCD[1]] = 1;
}

// AM off
void GF90970_am_off(void) {
    SEG_COM_GRID[AM_LCD[0]][AM_LCD[1]] = 0;
}

// PM on
void GF90970_pm_on(void) {
    SEG_COM_GRID[PM_LCD[0]][PM_LCD[1]] = 1;
}

// PM off
void GF90970_pm_off(void) {
    SEG_COM_GRID[PM_LCD[0]][PM_LCD[1]] = 0;
}

// Temperature Units C
void GF90970_temp_units_c(void) {
    //D7_LCD// for C
    //G7_LCD// for F
    SEG_COM_GRID[T4_LCD[0]][T4_LCD[1]] = 1;
    SEG_COM_GRID[D7_LCD[0]][D7_LCD[1]] = 1;
    SEG_COM_GRID[G7_LCD[0]][G7_LCD[1]] = 0;
}

void GF90970_temp_units_f(void) {
    //D7_LCD// for C
    //G7_LCD// for F
    SEG_COM_GRID[T4_LCD[0]][T4_LCD[1]] = 1;
    SEG_COM_GRID[D7_LCD[0]][D7_LCD[1]] = 0;
    SEG_COM_GRID[G7_LCD[0]][G7_LCD[1]] = 1;
}

void GF90970_temp_units_off(void) {
    SEG_COM_GRID[T4_LCD[0]][T4_LCD[1]] = 0;
    SEG_COM_GRID[D7_LCD[0]][D7_LCD[1]] = 0;
    SEG_COM_GRID[G7_LCD[0]][G7_LCD[1]] = 0;
}

// Temperature > 99
void GF90970_temp_over_ninety_nine_on(void) {
    SEG_COM_GRID[T3_LCD[0]][T3_LCD[1]] = 1;
}

// Temperature < 100
void GF90970_temp_over_ninety_nine_off(void) {
    SEG_COM_GRID[T3_LCD[0]][T3_LCD[1]] = 0;
}

// BT Detected
void GF90970_bt_detected_on(void) {
    SEG_COM_GRID[T8_LCD[0]][T8_LCD[1]] = 1;
}

// BT Not Detected
void GF90970_bt_detected_off(void) {
    SEG_COM_GRID[T8_LCD[0]][T8_LCD[1]] = 0;
}

// Column Divider for Main Clock On
void GF90970_main_time_div_on(void) {
    SEG_COM_GRID[COL1_LCD[0]][COL1_LCD[1]] = 1;
}

// Column Divider for Main Clock Off
void GF90970_main_time_div_off(void) {
    SEG_COM_GRID[COL1_LCD[0]][COL1_LCD[1]] = 0;
}

// Alarm Enabled Symbol On
void GF90970_alarm_set_on(void) {
    SEG_COM_GRID[T7_LCD[0]][T7_LCD[1]] = 1;
}

// Alarm Enabled Symbol Off
void GF90970_alarm_set_off(void) {
    SEG_COM_GRID[T7_LCD[0]][T7_LCD[1]] = 0;
}

// Nap Symbol On
void GF90970_nap_on(void) {
    SEG_COM_GRID[T5_LCD[0]][T5_LCD[1]] = 1;
}

// Nap Symbol Off
void GF90970_nap_off(void) {
    SEG_COM_GRID[T5_LCD[0]][T5_LCD[1]] = 0;
}

// Snooze Symbol On
void GF90970_snooze_on(void) {
    SEG_COM_GRID[T6_LCD[0]][T6_LCD[1]] = 1;
}

// Snooze Symbol Off
void GF90970_snooze_off(void) {
    SEG_COM_GRID[T6_LCD[0]][T6_LCD[1]] = 0;
}

// Alarm Time Column Divider On
void GF90970_alarm_time_div_on(void) {
    SEG_COM_GRID[COL2_LCD[0]][COL2_LCD[1]] = 1;
}

// Alarm Time Column Divider Off
void GF90970_alarm_time_div_off(void) {
    SEG_COM_GRID[COL2_LCD[0]][COL2_LCD[1]] = 0;
}

// Wifi Detected Symbol On
void GF90970_wifi_detected_on(void) {
    SEG_COM_GRID[T9_LCD[0]][T9_LCD[1]] = 1;
}

// Wifi Detected Symbol Off
void GF90970_wifi_detected_off(void) {
    SEG_COM_GRID[T9_LCD[0]][T9_LCD[1]] = 0;
}

// 'A', 'B', 'C', 'D', 'E', 'F', 'G'
void GF90970_number_0(int digit_num) {
    // For which digit
    switch (digit_num) {
    case 1:
        SEG_COM_GRID[A1_LCD[0]][A1_LCD[1]] = 1;
        SEG_COM_GRID[B1_LCD[0]][B1_LCD[1]] = 1;
        SEG_COM_GRID[C1_LCD[0]][C1_LCD[1]] = 1;
        SEG_COM_GRID[D1_LCD[0]][D1_LCD[1]] = 1;
        SEG_COM_GRID[E1_LCD[0]][E1_LCD[1]] = 1;
        SEG_COM_GRID[F1_LCD[0]][F1_LCD[1]] = 1;
        break;
    case 2:
        SEG_COM_GRID[A2_LCD[0]][A2_LCD[1]] = 1;
        SEG_COM_GRID[B2_LCD[0]][B2_LCD[1]] = 1;
        SEG_COM_GRID[C2_LCD[0]][C2_LCD[1]] = 1;
        SEG_COM_GRID[D2_LCD[0]][D2_LCD[1]] = 1;
        SEG_COM_GRID[E2_LCD[0]][E2_LCD[1]] = 1;
        SEG_COM_GRID[F2_LCD[0]][F2_LCD[1]] = 1;
        break;
    case 3:
        SEG_COM_GRID[A3_LCD[0]][A3_LCD[1]] = 1;
        SEG_COM_GRID[B3_LCD[0]][B3_LCD[1]] = 1;
        SEG_COM_GRID[C3_LCD[0]][C3_LCD[1]] = 1;
        SEG_COM_GRID[D3_LCD[0]][D3_LCD[1]] = 1;
        SEG_COM_GRID[E3_LCD[0]][E3_LCD[1]] = 1;
        SEG_COM_GRID[F3_LCD[0]][F3_LCD[1]] = 1;
        break;
    case 4:
        SEG_COM_GRID[A4_LCD[0]][A4_LCD[1]] = 1;
        SEG_COM_GRID[B4_LCD[0]][B4_LCD[1]] = 1;
        SEG_COM_GRID[C4_LCD[0]][C4_LCD[1]] = 1;
        SEG_COM_GRID[D4_LCD[0]][D4_LCD[1]] = 1;
        SEG_COM_GRID[E4_LCD[0]][E4_LCD[1]] = 1;
        SEG_COM_GRID[F4_LCD[0]][F4_LCD[1]] = 1;
        break;
    case 5:
        SEG_COM_GRID[A5_LCD[0]][A5_LCD[1]] = 1;
        SEG_COM_GRID[B5_LCD[0]][B5_LCD[1]] = 1;
        SEG_COM_GRID[C5_LCD[0]][C5_LCD[1]] = 1;
        SEG_COM_GRID[D5_LCD[0]][D5_LCD[1]] = 1;
        SEG_COM_GRID[E5_LCD[0]][E5_LCD[1]] = 1;
        SEG_COM_GRID[F5_LCD[0]][F5_LCD[1]] = 1;
        break;
    case 6:
        SEG_COM_GRID[A6_LCD[0]][A6_LCD[1]] = 1;
        SEG_COM_GRID[B6_LCD[0]][B6_LCD[1]] = 1;
        SEG_COM_GRID[C6_LCD[0]][C6_LCD[1]] = 1;
        SEG_COM_GRID[D6_LCD[0]][D6_LCD[1]] = 1;
        SEG_COM_GRID[E6_LCD[0]][E6_LCD[1]] = 1;
        SEG_COM_GRID[F6_LCD[0]][F6_LCD[1]] = 1;
        break;
    case 8:
        SEG_COM_GRID[A8_LCD[0]][A8_LCD[1]] = 1;
        SEG_COM_GRID[B8_LCD[0]][B8_LCD[1]] = 1;
        SEG_COM_GRID[C8_LCD[0]][C8_LCD[1]] = 1;
        SEG_COM_GRID[D8_LCD[0]][D8_LCD[1]] = 1;
        SEG_COM_GRID[E8_LCD[0]][E8_LCD[1]] = 1;
        SEG_COM_GRID[F8_LCD[0]][F8_LCD[1]] = 1;
        break;
    case 9:
        SEG_COM_GRID[A9_LCD[0]][A9_LCD[1]] = 1;
        SEG_COM_GRID[B9_LCD[0]][B9_LCD[1]] = 1;
        SEG_COM_GRID[C9_LCD[0]][C9_LCD[1]] = 1;
        SEG_COM_GRID[D9_LCD[0]][D9_LCD[1]] = 1;
        SEG_COM_GRID[E9_LCD[0]][E9_LCD[1]] = 1;
        SEG_COM_GRID[F9_LCD[0]][F9_LCD[1]] = 1;
        break;
    case 10:
        SEG_COM_GRID[A10_LCD[0]][A10_LCD[1]] = 1;
        SEG_COM_GRID[B10_LCD[0]][B10_LCD[1]] = 1;
        SEG_COM_GRID[C10_LCD[0]][C10_LCD[1]] = 1;
        SEG_COM_GRID[D10_LCD[0]][D10_LCD[1]] = 1;
        SEG_COM_GRID[E10_LCD[0]][E10_LCD[1]] = 1;
        SEG_COM_GRID[F10_LCD[0]][F10_LCD[1]] = 1;
        break;
    case 11:
        SEG_COM_GRID[A11_LCD[0]][A11_LCD[1]] = 1;
        SEG_COM_GRID[B11_LCD[0]][B11_LCD[1]] = 1;
        SEG_COM_GRID[C11_LCD[0]][C11_LCD[1]] = 1;
        SEG_COM_GRID[D11_LCD[0]][D11_LCD[1]] = 1;
        SEG_COM_GRID[E11_LCD[0]][E11_LCD[1]] = 1;
        SEG_COM_GRID[F11_LCD[0]][F11_LCD[1]] = 1;
        break;
    }
}

// 'B', 'C'
void GF90970_number_1(int digit_num) {
    // For which digit
    switch (digit_num) {
    case 1:
        SEG_COM_GRID[B1_LCD[0]][B1_LCD[1]] = 1;
        SEG_COM_GRID[C1_LCD[0]][C1_LCD[1]] = 1;
        break;
    case 2:
        SEG_COM_GRID[B2_LCD[0]][B2_LCD[1]] = 1;
        SEG_COM_GRID[C2_LCD[0]][C2_LCD[1]] = 1;
        break;
    case 3:
        SEG_COM_GRID[B3_LCD[0]][B3_LCD[1]] = 1;
        SEG_COM_GRID[C3_LCD[0]][C3_LCD[1]] = 1;
        break;
    case 4:
        SEG_COM_GRID[B4_LCD[0]][B4_LCD[1]] = 1;
        SEG_COM_GRID[C4_LCD[0]][C4_LCD[1]] = 1;
        break;
    case 5:
        SEG_COM_GRID[B5_LCD[0]][B5_LCD[1]] = 1;
        SEG_COM_GRID[C5_LCD[0]][C5_LCD[1]] = 1;
        break;
    case 6:
        SEG_COM_GRID[B6_LCD[0]][B6_LCD[1]] = 1;
        SEG_COM_GRID[C6_LCD[0]][C6_LCD[1]] = 1;
        break;
    case 8:
        SEG_COM_GRID[B8_LCD[0]][B8_LCD[1]] = 1;
        SEG_COM_GRID[C8_LCD[0]][C8_LCD[1]] = 1;
        break;
    case 9:
        SEG_COM_GRID[B9_LCD[0]][B9_LCD[1]] = 1;
        SEG_COM_GRID[C9_LCD[0]][C9_LCD[1]] = 1;
        break;
    case 10:
        SEG_COM_GRID[B10_LCD[0]][B10_LCD[1]] = 1;
        SEG_COM_GRID[C10_LCD[0]][C10_LCD[1]] = 1;
        break;
    case 11:
        SEG_COM_GRID[B11_LCD[0]][B11_LCD[1]] = 1;
        SEG_COM_GRID[C11_LCD[0]][C11_LCD[1]] = 1;
        break;
    }
}

// 'A', 'B', 'D', 'E', 'G'
void GF90970_number_2(int digit_num) {
    // For which digit
    switch (digit_num) {
    case 1:
        SEG_COM_GRID[A1_LCD[0]][A1_LCD[1]] = 1;
        SEG_COM_GRID[B1_LCD[0]][B1_LCD[1]] = 1;
        SEG_COM_GRID[D1_LCD[0]][D1_LCD[1]] = 1;
        SEG_COM_GRID[E1_LCD[0]][E1_LCD[1]] = 1;
        SEG_COM_GRID[G1_LCD[0]][G1_LCD[1]] = 1;
        break;
    case 2:
        SEG_COM_GRID[A2_LCD[0]][A2_LCD[1]] = 1;
        SEG_COM_GRID[B2_LCD[0]][B2_LCD[1]] = 1;
        SEG_COM_GRID[D2_LCD[0]][D2_LCD[1]] = 1;
        SEG_COM_GRID[E2_LCD[0]][E2_LCD[1]] = 1;
        SEG_COM_GRID[G2_LCD[0]][G2_LCD[1]] = 1;
        break;
    case 3:
        SEG_COM_GRID[A3_LCD[0]][A3_LCD[1]] = 1;
        SEG_COM_GRID[B3_LCD[0]][B3_LCD[1]] = 1;
        SEG_COM_GRID[D3_LCD[0]][D3_LCD[1]] = 1;
        SEG_COM_GRID[E3_LCD[0]][E3_LCD[1]] = 1;
        SEG_COM_GRID[G3_LCD[0]][G3_LCD[1]] = 1;
        break;
    case 4:
        SEG_COM_GRID[A4_LCD[0]][A4_LCD[1]] = 1;
        SEG_COM_GRID[B4_LCD[0]][B4_LCD[1]] = 1;
        SEG_COM_GRID[D4_LCD[0]][D4_LCD[1]] = 1;
        SEG_COM_GRID[E4_LCD[0]][E4_LCD[1]] = 1;
        SEG_COM_GRID[G4_LCD[0]][G4_LCD[1]] = 1;
        break;
    case 5:
        SEG_COM_GRID[A5_LCD[0]][A5_LCD[1]] = 1;
        SEG_COM_GRID[B5_LCD[0]][B5_LCD[1]] = 1;
        SEG_COM_GRID[D5_LCD[0]][D5_LCD[1]] = 1;
        SEG_COM_GRID[E5_LCD[0]][E5_LCD[1]] = 1;
        SEG_COM_GRID[G5_LCD[0]][G5_LCD[1]] = 1;
        break;
    case 6:
        SEG_COM_GRID[A6_LCD[0]][A6_LCD[1]] = 1;
        SEG_COM_GRID[B6_LCD[0]][B6_LCD[1]] = 1;
        SEG_COM_GRID[D6_LCD[0]][D6_LCD[1]] = 1;
        SEG_COM_GRID[E6_LCD[0]][E6_LCD[1]] = 1;
        SEG_COM_GRID[G6_LCD[0]][G6_LCD[1]] = 1;
        break;
    case 8:
        SEG_COM_GRID[A8_LCD[0]][A8_LCD[1]] = 1;
        SEG_COM_GRID[B8_LCD[0]][B8_LCD[1]] = 1;
        SEG_COM_GRID[D8_LCD[0]][D8_LCD[1]] = 1;
        SEG_COM_GRID[E8_LCD[0]][E8_LCD[1]] = 1;
        SEG_COM_GRID[G8_LCD[0]][G8_LCD[1]] = 1;
        break;
    case 9:
        SEG_COM_GRID[A9_LCD[0]][A9_LCD[1]] = 1;
        SEG_COM_GRID[B9_LCD[0]][B9_LCD[1]] = 1;
        SEG_COM_GRID[D9_LCD[0]][D9_LCD[1]] = 1;
        SEG_COM_GRID[E9_LCD[0]][E9_LCD[1]] = 1;
        SEG_COM_GRID[G9_LCD[0]][G9_LCD[1]] = 1;
        break;
    case 10:
        SEG_COM_GRID[A10_LCD[0]][A10_LCD[1]] = 1;
        SEG_COM_GRID[B10_LCD[0]][B10_LCD[1]] = 1;
        SEG_COM_GRID[D10_LCD[0]][D10_LCD[1]] = 1;
        SEG_COM_GRID[E10_LCD[0]][E10_LCD[1]] = 1;
        SEG_COM_GRID[G10_LCD[0]][G10_LCD[1]] = 1;
        break;
    case 11:
        SEG_COM_GRID[A11_LCD[0]][A11_LCD[1]] = 1;
        SEG_COM_GRID[B11_LCD[0]][B11_LCD[1]] = 1;
        SEG_COM_GRID[D11_LCD[0]][D11_LCD[1]] = 1;
        SEG_COM_GRID[E11_LCD[0]][E11_LCD[1]] = 1;
        SEG_COM_GRID[G11_LCD[0]][G11_LCD[1]] = 1;
        break;
    }
}

// 'A', 'B', 'C', 'D', 'G'
void GF90970_number_3(int digit_num) {
    // For which digit
    switch (digit_num) {
    case 1:
        SEG_COM_GRID[A1_LCD[0]][A1_LCD[1]] = 1;
        SEG_COM_GRID[B1_LCD[0]][B1_LCD[1]] = 1;
        SEG_COM_GRID[C1_LCD[0]][C1_LCD[1]] = 1;
        SEG_COM_GRID[D1_LCD[0]][D1_LCD[1]] = 1;
        SEG_COM_GRID[G1_LCD[0]][G1_LCD[1]] = 1;
        break;
    case 2:
        SEG_COM_GRID[A2_LCD[0]][A2_LCD[1]] = 1;
        SEG_COM_GRID[B2_LCD[0]][B2_LCD[1]] = 1;
        SEG_COM_GRID[C2_LCD[0]][C2_LCD[1]] = 1;
        SEG_COM_GRID[D2_LCD[0]][D2_LCD[1]] = 1;
        SEG_COM_GRID[G2_LCD[0]][G2_LCD[1]] = 1;
        break;
    case 3:
        SEG_COM_GRID[A3_LCD[0]][A3_LCD[1]] = 1;
        SEG_COM_GRID[B3_LCD[0]][B3_LCD[1]] = 1;
        SEG_COM_GRID[C3_LCD[0]][C3_LCD[1]] = 1;
        SEG_COM_GRID[D3_LCD[0]][D3_LCD[1]] = 1;
        SEG_COM_GRID[G3_LCD[0]][G3_LCD[1]] = 1;
        break;
    case 4:
        SEG_COM_GRID[A4_LCD[0]][A4_LCD[1]] = 1;
        SEG_COM_GRID[B4_LCD[0]][B4_LCD[1]] = 1;
        SEG_COM_GRID[C4_LCD[0]][C4_LCD[1]] = 1;
        SEG_COM_GRID[D4_LCD[0]][D4_LCD[1]] = 1;
        SEG_COM_GRID[G4_LCD[0]][G4_LCD[1]] = 1;
        break;
    case 5:
        SEG_COM_GRID[A5_LCD[0]][A5_LCD[1]] = 1;
        SEG_COM_GRID[B5_LCD[0]][B5_LCD[1]] = 1;
        SEG_COM_GRID[C5_LCD[0]][C5_LCD[1]] = 1;
        SEG_COM_GRID[D5_LCD[0]][D5_LCD[1]] = 1;
        SEG_COM_GRID[G5_LCD[0]][G5_LCD[1]] = 1;
        break;
    case 6:
        SEG_COM_GRID[A6_LCD[0]][A6_LCD[1]] = 1;
        SEG_COM_GRID[B6_LCD[0]][B6_LCD[1]] = 1;
        SEG_COM_GRID[C6_LCD[0]][C6_LCD[1]] = 1;
        SEG_COM_GRID[D6_LCD[0]][D6_LCD[1]] = 1;
        SEG_COM_GRID[G6_LCD[0]][G6_LCD[1]] = 1;
        break;
    case 8:
        SEG_COM_GRID[A8_LCD[0]][A8_LCD[1]] = 1;
        SEG_COM_GRID[B8_LCD[0]][B8_LCD[1]] = 1;
        SEG_COM_GRID[C8_LCD[0]][C8_LCD[1]] = 1;
        SEG_COM_GRID[D8_LCD[0]][D8_LCD[1]] = 1;
        SEG_COM_GRID[G8_LCD[0]][G8_LCD[1]] = 1;
        break;
    case 9:
        SEG_COM_GRID[A9_LCD[0]][A9_LCD[1]] = 1;
        SEG_COM_GRID[B9_LCD[0]][B9_LCD[1]] = 1;
        SEG_COM_GRID[C9_LCD[0]][C9_LCD[1]] = 1;
        SEG_COM_GRID[D9_LCD[0]][D9_LCD[1]] = 1;
        SEG_COM_GRID[G9_LCD[0]][G9_LCD[1]] = 1;
        break;
    case 10:
        SEG_COM_GRID[A10_LCD[0]][A10_LCD[1]] = 1;
        SEG_COM_GRID[B10_LCD[0]][B10_LCD[1]] = 1;
        SEG_COM_GRID[C10_LCD[0]][C10_LCD[1]] = 1;
        SEG_COM_GRID[D10_LCD[0]][D10_LCD[1]] = 1;
        SEG_COM_GRID[G10_LCD[0]][G10_LCD[1]] = 1;
        break;
    case 11:
        SEG_COM_GRID[A11_LCD[0]][A11_LCD[1]] = 1;
        SEG_COM_GRID[B11_LCD[0]][B11_LCD[1]] = 1;
        SEG_COM_GRID[C11_LCD[0]][C11_LCD[1]] = 1;
        SEG_COM_GRID[D11_LCD[0]][D11_LCD[1]] = 1;
        SEG_COM_GRID[G11_LCD[0]][G11_LCD[1]] = 1;
        break;
    }
}

// 'B', 'C', 'F', 'G'
void GF90970_number_4(int digit_num) {
    // For which digit
    switch (digit_num) {
    case 1:
        SEG_COM_GRID[B1_LCD[0]][B1_LCD[1]] = 1;
        SEG_COM_GRID[C1_LCD[0]][C1_LCD[1]] = 1;
        SEG_COM_GRID[F1_LCD[0]][F1_LCD[1]] = 1;
        SEG_COM_GRID[G1_LCD[0]][G1_LCD[1]] = 1;
        break;
    case 2:
        SEG_COM_GRID[B2_LCD[0]][B2_LCD[1]] = 1;
        SEG_COM_GRID[C2_LCD[0]][C2_LCD[1]] = 1;
        SEG_COM_GRID[F2_LCD[0]][F2_LCD[1]] = 1;
        SEG_COM_GRID[G2_LCD[0]][G2_LCD[1]] = 1;
        break;
    case 3:
        SEG_COM_GRID[B3_LCD[0]][B3_LCD[1]] = 1;
        SEG_COM_GRID[C3_LCD[0]][C3_LCD[1]] = 1;
        SEG_COM_GRID[F3_LCD[0]][F3_LCD[1]] = 1;
        SEG_COM_GRID[G3_LCD[0]][G3_LCD[1]] = 1;
        break;
    case 4:
        SEG_COM_GRID[B4_LCD[0]][B4_LCD[1]] = 1;
        SEG_COM_GRID[C4_LCD[0]][C4_LCD[1]] = 1;
        SEG_COM_GRID[F4_LCD[0]][F4_LCD[1]] = 1;
        SEG_COM_GRID[G4_LCD[0]][G4_LCD[1]] = 1;
        break;
    case 5:
        SEG_COM_GRID[B5_LCD[0]][B5_LCD[1]] = 1;
        SEG_COM_GRID[C5_LCD[0]][C5_LCD[1]] = 1;
        SEG_COM_GRID[F5_LCD[0]][F5_LCD[1]] = 1;
        SEG_COM_GRID[G5_LCD[0]][G5_LCD[1]] = 1;
        break;
    case 6:
        SEG_COM_GRID[B6_LCD[0]][B6_LCD[1]] = 1;
        SEG_COM_GRID[C6_LCD[0]][C6_LCD[1]] = 1;
        SEG_COM_GRID[F6_LCD[0]][F6_LCD[1]] = 1;
        SEG_COM_GRID[G6_LCD[0]][G6_LCD[1]] = 1;
        break;
    case 8:
        SEG_COM_GRID[B8_LCD[0]][B8_LCD[1]] = 1;
        SEG_COM_GRID[C8_LCD[0]][C8_LCD[1]] = 1;
        SEG_COM_GRID[F8_LCD[0]][F8_LCD[1]] = 1;
        SEG_COM_GRID[G8_LCD[0]][G8_LCD[1]] = 1;
        break;
    case 9:
        SEG_COM_GRID[B9_LCD[0]][B9_LCD[1]] = 1;
        SEG_COM_GRID[C9_LCD[0]][C9_LCD[1]] = 1;
        SEG_COM_GRID[F9_LCD[0]][F9_LCD[1]] = 1;
        SEG_COM_GRID[G9_LCD[0]][G9_LCD[1]] = 1;
        break;
    case 10:
        SEG_COM_GRID[B10_LCD[0]][B10_LCD[1]] = 1;
        SEG_COM_GRID[C10_LCD[0]][C10_LCD[1]] = 1;
        SEG_COM_GRID[F10_LCD[0]][F10_LCD[1]] = 1;
        SEG_COM_GRID[G10_LCD[0]][G10_LCD[1]] = 1;
        break;
    case 11:
        SEG_COM_GRID[B11_LCD[0]][B11_LCD[1]] = 1;
        SEG_COM_GRID[C11_LCD[0]][C11_LCD[1]] = 1;
        SEG_COM_GRID[F11_LCD[0]][F11_LCD[1]] = 1;
        SEG_COM_GRID[G11_LCD[0]][G11_LCD[1]] = 1;
        break;
    }
}

// 'A', 'C', 'D', 'F', 'G'
void GF90970_number_5(int digit_num) {
    // For which digit
    switch (digit_num) {
    case 1:
        SEG_COM_GRID[A1_LCD[0]][A1_LCD[1]] = 1;
        SEG_COM_GRID[C1_LCD[0]][C1_LCD[1]] = 1;
        SEG_COM_GRID[D1_LCD[0]][D1_LCD[1]] = 1;
        SEG_COM_GRID[F1_LCD[0]][F1_LCD[1]] = 1;
        SEG_COM_GRID[G1_LCD[0]][G1_LCD[1]] = 1;
        break;
    case 2:
        SEG_COM_GRID[A2_LCD[0]][A2_LCD[1]] = 1;
        SEG_COM_GRID[C2_LCD[0]][C2_LCD[1]] = 1;
        SEG_COM_GRID[D2_LCD[0]][D2_LCD[1]] = 1;
        SEG_COM_GRID[F2_LCD[0]][F2_LCD[1]] = 1;
        SEG_COM_GRID[G2_LCD[0]][G2_LCD[1]] = 1;
        break;
    case 3:
        SEG_COM_GRID[A3_LCD[0]][A3_LCD[1]] = 1;
        SEG_COM_GRID[C3_LCD[0]][C3_LCD[1]] = 1;
        SEG_COM_GRID[D3_LCD[0]][D3_LCD[1]] = 1;
        SEG_COM_GRID[F3_LCD[0]][F3_LCD[1]] = 1;
        SEG_COM_GRID[G3_LCD[0]][G3_LCD[1]] = 1;
        break;
    case 4:
        SEG_COM_GRID[A4_LCD[0]][A4_LCD[1]] = 1;
        SEG_COM_GRID[C4_LCD[0]][C4_LCD[1]] = 1;
        SEG_COM_GRID[D4_LCD[0]][D4_LCD[1]] = 1;
        SEG_COM_GRID[F4_LCD[0]][F4_LCD[1]] = 1;
        SEG_COM_GRID[G4_LCD[0]][G4_LCD[1]] = 1;
        break;
    case 5:
        SEG_COM_GRID[A5_LCD[0]][A5_LCD[1]] = 1;
        SEG_COM_GRID[C5_LCD[0]][C5_LCD[1]] = 1;
        SEG_COM_GRID[D5_LCD[0]][D5_LCD[1]] = 1;
        SEG_COM_GRID[F5_LCD[0]][F5_LCD[1]] = 1;
        SEG_COM_GRID[G5_LCD[0]][G5_LCD[1]] = 1;
        break;
    case 6:
        SEG_COM_GRID[A6_LCD[0]][A6_LCD[1]] = 1;
        SEG_COM_GRID[C6_LCD[0]][C6_LCD[1]] = 1;
        SEG_COM_GRID[D6_LCD[0]][D6_LCD[1]] = 1;
        SEG_COM_GRID[F6_LCD[0]][F6_LCD[1]] = 1;
        SEG_COM_GRID[G6_LCD[0]][G6_LCD[1]] = 1;
        break;
    case 8:
        SEG_COM_GRID[A8_LCD[0]][A8_LCD[1]] = 1;
        SEG_COM_GRID[C8_LCD[0]][C8_LCD[1]] = 1;
        SEG_COM_GRID[D8_LCD[0]][D8_LCD[1]] = 1;
        SEG_COM_GRID[F8_LCD[0]][F8_LCD[1]] = 1;
        SEG_COM_GRID[G8_LCD[0]][G8_LCD[1]] = 1;
        break;
    case 9:
        SEG_COM_GRID[A9_LCD[0]][A9_LCD[1]] = 1;
        SEG_COM_GRID[C9_LCD[0]][C9_LCD[1]] = 1;
        SEG_COM_GRID[D9_LCD[0]][D9_LCD[1]] = 1;
        SEG_COM_GRID[F9_LCD[0]][F9_LCD[1]] = 1;
        SEG_COM_GRID[G9_LCD[0]][G9_LCD[1]] = 1;
        break;
    case 10:
        SEG_COM_GRID[A10_LCD[0]][A10_LCD[1]] = 1;
        SEG_COM_GRID[C10_LCD[0]][C10_LCD[1]] = 1;
        SEG_COM_GRID[D10_LCD[0]][D10_LCD[1]] = 1;
        SEG_COM_GRID[F10_LCD[0]][F10_LCD[1]] = 1;
        SEG_COM_GRID[G10_LCD[0]][G10_LCD[1]] = 1;
        break;
    case 11:
        SEG_COM_GRID[A11_LCD[0]][A11_LCD[1]] = 1;
        SEG_COM_GRID[C11_LCD[0]][C11_LCD[1]] = 1;
        SEG_COM_GRID[D11_LCD[0]][D11_LCD[1]] = 1;
        SEG_COM_GRID[F11_LCD[0]][F11_LCD[1]] = 1;
        SEG_COM_GRID[G11_LCD[0]][G11_LCD[1]] = 1;
        break;
    }
}

// 'A', 'C', 'D', 'E', 'F', 'G'
void GF90970_number_6(int digit_num) {
    // For which digit
    switch (digit_num) {
    case 1:
        SEG_COM_GRID[A1_LCD[0]][A1_LCD[1]] = 1;
        SEG_COM_GRID[C1_LCD[0]][C1_LCD[1]] = 1;
        SEG_COM_GRID[D1_LCD[0]][D1_LCD[1]] = 1;
        SEG_COM_GRID[E1_LCD[0]][E1_LCD[1]] = 1;
        SEG_COM_GRID[F1_LCD[0]][F1_LCD[1]] = 1;
        SEG_COM_GRID[G1_LCD[0]][G1_LCD[1]] = 1;
        break;
    case 2:
        SEG_COM_GRID[A2_LCD[0]][A2_LCD[1]] = 1;
        SEG_COM_GRID[C2_LCD[0]][C2_LCD[1]] = 1;
        SEG_COM_GRID[D2_LCD[0]][D2_LCD[1]] = 1;
        SEG_COM_GRID[E2_LCD[0]][E2_LCD[1]] = 1;
        SEG_COM_GRID[F2_LCD[0]][F2_LCD[1]] = 1;
        SEG_COM_GRID[G2_LCD[0]][G2_LCD[1]] = 1;
        break;
    case 3:
        SEG_COM_GRID[A3_LCD[0]][A3_LCD[1]] = 1;
        SEG_COM_GRID[C3_LCD[0]][C3_LCD[1]] = 1;
        SEG_COM_GRID[D3_LCD[0]][D3_LCD[1]] = 1;
        SEG_COM_GRID[E3_LCD[0]][E3_LCD[1]] = 1;
        SEG_COM_GRID[F3_LCD[0]][F3_LCD[1]] = 1;
        SEG_COM_GRID[G3_LCD[0]][G3_LCD[1]] = 1;
        break;
    case 4:
        SEG_COM_GRID[A4_LCD[0]][A4_LCD[1]] = 1;
        SEG_COM_GRID[C4_LCD[0]][C4_LCD[1]] = 1;
        SEG_COM_GRID[D4_LCD[0]][D4_LCD[1]] = 1;
        SEG_COM_GRID[E4_LCD[0]][E4_LCD[1]] = 1;
        SEG_COM_GRID[F4_LCD[0]][F4_LCD[1]] = 1;
        SEG_COM_GRID[G4_LCD[0]][G4_LCD[1]] = 1;
        break;
    case 5:
        SEG_COM_GRID[A5_LCD[0]][A5_LCD[1]] = 1;
        SEG_COM_GRID[C5_LCD[0]][C5_LCD[1]] = 1;
        SEG_COM_GRID[D5_LCD[0]][D5_LCD[1]] = 1;
        SEG_COM_GRID[E5_LCD[0]][E5_LCD[1]] = 1;
        SEG_COM_GRID[F5_LCD[0]][F5_LCD[1]] = 1;
        SEG_COM_GRID[G5_LCD[0]][G5_LCD[1]] = 1;
        break;
    case 6:
        SEG_COM_GRID[A6_LCD[0]][A6_LCD[1]] = 1;
        SEG_COM_GRID[C6_LCD[0]][C6_LCD[1]] = 1;
        SEG_COM_GRID[D6_LCD[0]][D6_LCD[1]] = 1;
        SEG_COM_GRID[E6_LCD[0]][E6_LCD[1]] = 1;
        SEG_COM_GRID[F6_LCD[0]][F6_LCD[1]] = 1;
        SEG_COM_GRID[G6_LCD[0]][G6_LCD[1]] = 1;
        break;
    case 8:
        SEG_COM_GRID[A8_LCD[0]][A8_LCD[1]] = 1;
        SEG_COM_GRID[C8_LCD[0]][C8_LCD[1]] = 1;
        SEG_COM_GRID[D8_LCD[0]][D8_LCD[1]] = 1;
        SEG_COM_GRID[E8_LCD[0]][E8_LCD[1]] = 1;
        SEG_COM_GRID[F8_LCD[0]][F8_LCD[1]] = 1;
        SEG_COM_GRID[G8_LCD[0]][G8_LCD[1]] = 1;
        break;
    case 9:
        SEG_COM_GRID[A9_LCD[0]][A9_LCD[1]] = 1;
        SEG_COM_GRID[C9_LCD[0]][C9_LCD[1]] = 1;
        SEG_COM_GRID[D9_LCD[0]][D9_LCD[1]] = 1;
        SEG_COM_GRID[E9_LCD[0]][E9_LCD[1]] = 1;
        SEG_COM_GRID[F9_LCD[0]][F9_LCD[1]] = 1;
        SEG_COM_GRID[G9_LCD[0]][G9_LCD[1]] = 1;
        break;
    case 10:
        SEG_COM_GRID[A10_LCD[0]][A10_LCD[1]] = 1;
        SEG_COM_GRID[C10_LCD[0]][C10_LCD[1]] = 1;
        SEG_COM_GRID[D10_LCD[0]][D10_LCD[1]] = 1;
        SEG_COM_GRID[E10_LCD[0]][E10_LCD[1]] = 1;
        SEG_COM_GRID[F10_LCD[0]][F10_LCD[1]] = 1;
        SEG_COM_GRID[G10_LCD[0]][G10_LCD[1]] = 1;
        break;
    case 11:
        SEG_COM_GRID[A11_LCD[0]][A11_LCD[1]] = 1;
        SEG_COM_GRID[C11_LCD[0]][C11_LCD[1]] = 1;
        SEG_COM_GRID[D11_LCD[0]][D11_LCD[1]] = 1;
        SEG_COM_GRID[E11_LCD[0]][E11_LCD[1]] = 1;
        SEG_COM_GRID[F11_LCD[0]][F11_LCD[1]] = 1;
        SEG_COM_GRID[G11_LCD[0]][G11_LCD[1]] = 1;
        break;
    }
}

// 'A', 'B', 'C'
void GF90970_number_7(int digit_num) {
    // For which digit
    switch (digit_num) {
    case 1:
        SEG_COM_GRID[A1_LCD[0]][A1_LCD[1]] = 1;
        SEG_COM_GRID[B1_LCD[0]][B1_LCD[1]] = 1;
        SEG_COM_GRID[C1_LCD[0]][C1_LCD[1]] = 1;
        break;
    case 2:
        SEG_COM_GRID[A2_LCD[0]][A2_LCD[1]] = 1;
        SEG_COM_GRID[B2_LCD[0]][B2_LCD[1]] = 1;
        SEG_COM_GRID[C2_LCD[0]][C2_LCD[1]] = 1;
        break;
    case 3:
        SEG_COM_GRID[A3_LCD[0]][A3_LCD[1]] = 1;
        SEG_COM_GRID[B3_LCD[0]][B3_LCD[1]] = 1;
        SEG_COM_GRID[C3_LCD[0]][C3_LCD[1]] = 1;
        break;
    case 4:
        SEG_COM_GRID[A4_LCD[0]][A4_LCD[1]] = 1;
        SEG_COM_GRID[B4_LCD[0]][B4_LCD[1]] = 1;
        SEG_COM_GRID[C4_LCD[0]][C4_LCD[1]] = 1;
        break;
    case 5:
        SEG_COM_GRID[A5_LCD[0]][A5_LCD[1]] = 1;
        SEG_COM_GRID[B5_LCD[0]][B5_LCD[1]] = 1;
        SEG_COM_GRID[C5_LCD[0]][C5_LCD[1]] = 1;
        break;
    case 6:
        SEG_COM_GRID[A6_LCD[0]][A6_LCD[1]] = 1;
        SEG_COM_GRID[B6_LCD[0]][B6_LCD[1]] = 1;
        SEG_COM_GRID[C6_LCD[0]][C6_LCD[1]] = 1;
        break;
    case 8:
        SEG_COM_GRID[A8_LCD[0]][A8_LCD[1]] = 1;
        SEG_COM_GRID[B8_LCD[0]][B8_LCD[1]] = 1;
        SEG_COM_GRID[C8_LCD[0]][C8_LCD[1]] = 1;
        break;
    case 9:
        SEG_COM_GRID[A9_LCD[0]][A9_LCD[1]] = 1;
        SEG_COM_GRID[B9_LCD[0]][B9_LCD[1]] = 1;
        SEG_COM_GRID[C9_LCD[0]][C9_LCD[1]] = 1;
        break;
    case 10:
        SEG_COM_GRID[A10_LCD[0]][A10_LCD[1]] = 1;
        SEG_COM_GRID[B10_LCD[0]][B10_LCD[1]] = 1;
        SEG_COM_GRID[C10_LCD[0]][C10_LCD[1]] = 1;
        break;
    case 11:
        SEG_COM_GRID[A11_LCD[0]][A11_LCD[1]] = 1;
        SEG_COM_GRID[B11_LCD[0]][B11_LCD[1]] = 1;
        SEG_COM_GRID[C11_LCD[0]][C11_LCD[1]] = 1;
        break;
    }
}

// 'A', 'B', 'C', 'D', 'E', 'F', 'G'
void GF90970_number_8(int digit_num) {
    // For which digit
    switch (digit_num) {
    case 1:
        SEG_COM_GRID[A1_LCD[0]][A1_LCD[1]] = 1;
        SEG_COM_GRID[B1_LCD[0]][B1_LCD[1]] = 1;
        SEG_COM_GRID[C1_LCD[0]][C1_LCD[1]] = 1;
        SEG_COM_GRID[D1_LCD[0]][D1_LCD[1]] = 1;
        SEG_COM_GRID[E1_LCD[0]][E1_LCD[1]] = 1;
        SEG_COM_GRID[F1_LCD[0]][F1_LCD[1]] = 1;
        SEG_COM_GRID[G1_LCD[0]][G1_LCD[1]] = 1;
        break;
    case 2:
        SEG_COM_GRID[A2_LCD[0]][A2_LCD[1]] = 1;
        SEG_COM_GRID[B2_LCD[0]][B2_LCD[1]] = 1;
        SEG_COM_GRID[C2_LCD[0]][C2_LCD[1]] = 1;
        SEG_COM_GRID[D2_LCD[0]][D2_LCD[1]] = 1;
        SEG_COM_GRID[E2_LCD[0]][E2_LCD[1]] = 1;
        SEG_COM_GRID[F2_LCD[0]][F2_LCD[1]] = 1;
        SEG_COM_GRID[G2_LCD[0]][G2_LCD[1]] = 1;
        break;
    case 3:
        SEG_COM_GRID[A3_LCD[0]][A3_LCD[1]] = 1;
        SEG_COM_GRID[B3_LCD[0]][B3_LCD[1]] = 1;
        SEG_COM_GRID[C3_LCD[0]][C3_LCD[1]] = 1;
        SEG_COM_GRID[D3_LCD[0]][D3_LCD[1]] = 1;
        SEG_COM_GRID[E3_LCD[0]][E3_LCD[1]] = 1;
        SEG_COM_GRID[F3_LCD[0]][F3_LCD[1]] = 1;
        SEG_COM_GRID[G3_LCD[0]][G3_LCD[1]] = 1;
        break;
    case 4:
        SEG_COM_GRID[A4_LCD[0]][A4_LCD[1]] = 1;
        SEG_COM_GRID[B4_LCD[0]][B4_LCD[1]] = 1;
        SEG_COM_GRID[C4_LCD[0]][C4_LCD[1]] = 1;
        SEG_COM_GRID[D4_LCD[0]][D4_LCD[1]] = 1;
        SEG_COM_GRID[E4_LCD[0]][E4_LCD[1]] = 1;
        SEG_COM_GRID[F4_LCD[0]][F4_LCD[1]] = 1;
        SEG_COM_GRID[G4_LCD[0]][G4_LCD[1]] = 1;
        break;
    case 5:
        SEG_COM_GRID[A5_LCD[0]][A5_LCD[1]] = 1;
        SEG_COM_GRID[B5_LCD[0]][B5_LCD[1]] = 1;
        SEG_COM_GRID[C5_LCD[0]][C5_LCD[1]] = 1;
        SEG_COM_GRID[D5_LCD[0]][D5_LCD[1]] = 1;
        SEG_COM_GRID[E5_LCD[0]][E5_LCD[1]] = 1;
        SEG_COM_GRID[F5_LCD[0]][F5_LCD[1]] = 1;
        SEG_COM_GRID[G5_LCD[0]][G5_LCD[1]] = 1;
        break;
    case 6:
        SEG_COM_GRID[A6_LCD[0]][A6_LCD[1]] = 1;
        SEG_COM_GRID[B6_LCD[0]][B6_LCD[1]] = 1;
        SEG_COM_GRID[C6_LCD[0]][C6_LCD[1]] = 1;
        SEG_COM_GRID[D6_LCD[0]][D6_LCD[1]] = 1;
        SEG_COM_GRID[E6_LCD[0]][E6_LCD[1]] = 1;
        SEG_COM_GRID[F6_LCD[0]][F6_LCD[1]] = 1;
        SEG_COM_GRID[G6_LCD[0]][G6_LCD[1]] = 1;
        break;
    case 8:
        SEG_COM_GRID[A8_LCD[0]][A8_LCD[1]] = 1;
        SEG_COM_GRID[B8_LCD[0]][B8_LCD[1]] = 1;
        SEG_COM_GRID[C8_LCD[0]][C8_LCD[1]] = 1;
        SEG_COM_GRID[D8_LCD[0]][D8_LCD[1]] = 1;
        SEG_COM_GRID[E8_LCD[0]][E8_LCD[1]] = 1;
        SEG_COM_GRID[F8_LCD[0]][F8_LCD[1]] = 1;
        SEG_COM_GRID[G8_LCD[0]][G8_LCD[1]] = 1;
        break;
    case 9:
        SEG_COM_GRID[A9_LCD[0]][A9_LCD[1]] = 1;
        SEG_COM_GRID[B9_LCD[0]][B9_LCD[1]] = 1;
        SEG_COM_GRID[C9_LCD[0]][C9_LCD[1]] = 1;
        SEG_COM_GRID[D9_LCD[0]][D9_LCD[1]] = 1;
        SEG_COM_GRID[E9_LCD[0]][E9_LCD[1]] = 1;
        SEG_COM_GRID[F9_LCD[0]][F9_LCD[1]] = 1;
        SEG_COM_GRID[G9_LCD[0]][G9_LCD[1]] = 1;
        break;
    case 10:
        SEG_COM_GRID[A10_LCD[0]][A10_LCD[1]] = 1;
        SEG_COM_GRID[B10_LCD[0]][B10_LCD[1]] = 1;
        SEG_COM_GRID[C10_LCD[0]][C10_LCD[1]] = 1;
        SEG_COM_GRID[D10_LCD[0]][D10_LCD[1]] = 1;
        SEG_COM_GRID[E10_LCD[0]][E10_LCD[1]] = 1;
        SEG_COM_GRID[F10_LCD[0]][F10_LCD[1]] = 1;
        SEG_COM_GRID[G10_LCD[0]][G10_LCD[1]] = 1;
        break;
    case 11:
        SEG_COM_GRID[A11_LCD[0]][A11_LCD[1]] = 1;
        SEG_COM_GRID[B11_LCD[0]][B11_LCD[1]] = 1;
        SEG_COM_GRID[C11_LCD[0]][C11_LCD[1]] = 1;
        SEG_COM_GRID[D11_LCD[0]][D11_LCD[1]] = 1;
        SEG_COM_GRID[E11_LCD[0]][E11_LCD[1]] = 1;
        SEG_COM_GRID[F11_LCD[0]][F11_LCD[1]] = 1;
        SEG_COM_GRID[G11_LCD[0]][G11_LCD[1]] = 1;
        break;
    }
}

// 'A', 'B', 'C', 'F', 'G'
void GF90970_number_9(int digit_num) {
    // For which digit
    switch (digit_num) {
    case 1:
        SEG_COM_GRID[A1_LCD[0]][A1_LCD[1]] = 1;
        SEG_COM_GRID[B1_LCD[0]][B1_LCD[1]] = 1;
        SEG_COM_GRID[C1_LCD[0]][C1_LCD[1]] = 1;
        SEG_COM_GRID[D1_LCD[0]][D1_LCD[1]] = 1;
        SEG_COM_GRID[F1_LCD[0]][F1_LCD[1]] = 1;
        SEG_COM_GRID[G1_LCD[0]][G1_LCD[1]] = 1;
        break;
    case 2:
        SEG_COM_GRID[A2_LCD[0]][A2_LCD[1]] = 1;
        SEG_COM_GRID[B2_LCD[0]][B2_LCD[1]] = 1;
        SEG_COM_GRID[C2_LCD[0]][C2_LCD[1]] = 1;
        SEG_COM_GRID[D2_LCD[0]][D2_LCD[1]] = 1;
        SEG_COM_GRID[F2_LCD[0]][F2_LCD[1]] = 1;
        SEG_COM_GRID[G2_LCD[0]][G2_LCD[1]] = 1;
        break;
    case 3:
        SEG_COM_GRID[A3_LCD[0]][A3_LCD[1]] = 1;
        SEG_COM_GRID[B3_LCD[0]][B3_LCD[1]] = 1;
        SEG_COM_GRID[C3_LCD[0]][C3_LCD[1]] = 1;
        SEG_COM_GRID[D3_LCD[0]][D3_LCD[1]] = 1;
        SEG_COM_GRID[F3_LCD[0]][F3_LCD[1]] = 1;
        SEG_COM_GRID[G3_LCD[0]][G3_LCD[1]] = 1;
        break;
    case 4:
        SEG_COM_GRID[A4_LCD[0]][A4_LCD[1]] = 1;
        SEG_COM_GRID[B4_LCD[0]][B4_LCD[1]] = 1;
        SEG_COM_GRID[C4_LCD[0]][C4_LCD[1]] = 1;
        SEG_COM_GRID[D4_LCD[0]][D4_LCD[1]] = 1;
        SEG_COM_GRID[F4_LCD[0]][F4_LCD[1]] = 1;
        SEG_COM_GRID[G4_LCD[0]][G4_LCD[1]] = 1;
        break;
    case 5:
        SEG_COM_GRID[A5_LCD[0]][A5_LCD[1]] = 1;
        SEG_COM_GRID[B5_LCD[0]][B5_LCD[1]] = 1;
        SEG_COM_GRID[C5_LCD[0]][C5_LCD[1]] = 1;
        SEG_COM_GRID[D5_LCD[0]][D5_LCD[1]] = 1;
        SEG_COM_GRID[F5_LCD[0]][F5_LCD[1]] = 1;
        SEG_COM_GRID[G5_LCD[0]][G5_LCD[1]] = 1;
        break;
    case 6:
        SEG_COM_GRID[A6_LCD[0]][A6_LCD[1]] = 1;
        SEG_COM_GRID[B6_LCD[0]][B6_LCD[1]] = 1;
        SEG_COM_GRID[C6_LCD[0]][C6_LCD[1]] = 1;
        SEG_COM_GRID[D6_LCD[0]][D6_LCD[1]] = 1;
        SEG_COM_GRID[F6_LCD[0]][F6_LCD[1]] = 1;
        SEG_COM_GRID[G6_LCD[0]][G6_LCD[1]] = 1;
        break;
    case 8:
        SEG_COM_GRID[A8_LCD[0]][A8_LCD[1]] = 1;
        SEG_COM_GRID[B8_LCD[0]][B8_LCD[1]] = 1;
        SEG_COM_GRID[C8_LCD[0]][C8_LCD[1]] = 1;
        SEG_COM_GRID[D8_LCD[0]][D8_LCD[1]] = 1;
        SEG_COM_GRID[F8_LCD[0]][F8_LCD[1]] = 1;
        SEG_COM_GRID[G8_LCD[0]][G8_LCD[1]] = 1;
        break;
    case 9:
        SEG_COM_GRID[A9_LCD[0]][A9_LCD[1]] = 1;
        SEG_COM_GRID[B9_LCD[0]][B9_LCD[1]] = 1;
        SEG_COM_GRID[C9_LCD[0]][C9_LCD[1]] = 1;
        SEG_COM_GRID[D9_LCD[0]][D9_LCD[1]] = 1;
        SEG_COM_GRID[F9_LCD[0]][F9_LCD[1]] = 1;
        SEG_COM_GRID[G9_LCD[0]][G9_LCD[1]] = 1;
        break;
    case 10:
        SEG_COM_GRID[A10_LCD[0]][A10_LCD[1]] = 1;
        SEG_COM_GRID[B10_LCD[0]][B10_LCD[1]] = 1;
        SEG_COM_GRID[C10_LCD[0]][C10_LCD[1]] = 1;
        SEG_COM_GRID[D10_LCD[0]][D10_LCD[1]] = 1;
        SEG_COM_GRID[F10_LCD[0]][F10_LCD[1]] = 1;
        SEG_COM_GRID[G10_LCD[0]][G10_LCD[1]] = 1;
        break;
    case 11:
        SEG_COM_GRID[A11_LCD[0]][A11_LCD[1]] = 1;
        SEG_COM_GRID[B11_LCD[0]][B11_LCD[1]] = 1;
        SEG_COM_GRID[C11_LCD[0]][C11_LCD[1]] = 1;
        SEG_COM_GRID[D11_LCD[0]][D11_LCD[1]] = 1;
        SEG_COM_GRID[F11_LCD[0]][F11_LCD[1]] = 1;
        SEG_COM_GRID[G11_LCD[0]][G11_LCD[1]] = 1;
        break;
    }
}







// 'A', 'B', 'C', 'D', 'E', 'F', 'G'
void GF90970_clear_digit(int digit_num) {
    // For which digit
    switch (digit_num) {
    case 1:
        SEG_COM_GRID[A1_LCD[0]][A1_LCD[1]] = 0;
        SEG_COM_GRID[B1_LCD[0]][B1_LCD[1]] = 0;
        SEG_COM_GRID[C1_LCD[0]][C1_LCD[1]] = 0;
        SEG_COM_GRID[D1_LCD[0]][D1_LCD[1]] = 0;
        SEG_COM_GRID[E1_LCD[0]][E1_LCD[1]] = 0;
        SEG_COM_GRID[F1_LCD[0]][F1_LCD[1]] = 0;
        SEG_COM_GRID[G1_LCD[0]][G1_LCD[1]] = 0;
        break;
    case 2:
        SEG_COM_GRID[A2_LCD[0]][A2_LCD[1]] = 0;
        SEG_COM_GRID[B2_LCD[0]][B2_LCD[1]] = 0;
        SEG_COM_GRID[C2_LCD[0]][C2_LCD[1]] = 0;
        SEG_COM_GRID[D2_LCD[0]][D2_LCD[1]] = 0;
        SEG_COM_GRID[E2_LCD[0]][E2_LCD[1]] = 0;
        SEG_COM_GRID[F2_LCD[0]][F2_LCD[1]] = 0;
        SEG_COM_GRID[G2_LCD[0]][G2_LCD[1]] = 0;
        break;
    case 3:
        SEG_COM_GRID[A3_LCD[0]][A3_LCD[1]] = 0;
        SEG_COM_GRID[B3_LCD[0]][B3_LCD[1]] = 0;
        SEG_COM_GRID[C3_LCD[0]][C3_LCD[1]] = 0;
        SEG_COM_GRID[D3_LCD[0]][D3_LCD[1]] = 0;
        SEG_COM_GRID[E3_LCD[0]][E3_LCD[1]] = 0;
        SEG_COM_GRID[F3_LCD[0]][F3_LCD[1]] = 0;
        SEG_COM_GRID[G3_LCD[0]][G3_LCD[1]] = 0;
        break;
    case 4:
        SEG_COM_GRID[A4_LCD[0]][A4_LCD[1]] = 0;
        SEG_COM_GRID[B4_LCD[0]][B4_LCD[1]] = 0;
        SEG_COM_GRID[C4_LCD[0]][C4_LCD[1]] = 0;
        SEG_COM_GRID[D4_LCD[0]][D4_LCD[1]] = 0;
        SEG_COM_GRID[E4_LCD[0]][E4_LCD[1]] = 0;
        SEG_COM_GRID[F4_LCD[0]][F4_LCD[1]] = 0;
        SEG_COM_GRID[G4_LCD[0]][G4_LCD[1]] = 0;
        break;
    case 5:
        SEG_COM_GRID[A5_LCD[0]][A5_LCD[1]] = 0;
        SEG_COM_GRID[B5_LCD[0]][B5_LCD[1]] = 0;
        SEG_COM_GRID[C5_LCD[0]][C5_LCD[1]] = 0;
        SEG_COM_GRID[D5_LCD[0]][D5_LCD[1]] = 0;
        SEG_COM_GRID[E5_LCD[0]][E5_LCD[1]] = 0;
        SEG_COM_GRID[F5_LCD[0]][F5_LCD[1]] = 0;
        SEG_COM_GRID[G5_LCD[0]][G5_LCD[1]] = 0;
        break;
    case 6:
        SEG_COM_GRID[A6_LCD[0]][A6_LCD[1]] = 0;
        SEG_COM_GRID[B6_LCD[0]][B6_LCD[1]] = 0;
        SEG_COM_GRID[C6_LCD[0]][C6_LCD[1]] = 0;
        SEG_COM_GRID[D6_LCD[0]][D6_LCD[1]] = 0;
        SEG_COM_GRID[E6_LCD[0]][E6_LCD[1]] = 0;
        SEG_COM_GRID[F6_LCD[0]][F6_LCD[1]] = 0;
        SEG_COM_GRID[G6_LCD[0]][G6_LCD[1]] = 0;
        break;
    case 8:
        SEG_COM_GRID[A8_LCD[0]][A8_LCD[1]] = 0;
        SEG_COM_GRID[B8_LCD[0]][B8_LCD[1]] = 0;
        SEG_COM_GRID[C8_LCD[0]][C8_LCD[1]] = 0;
        SEG_COM_GRID[D8_LCD[0]][D8_LCD[1]] = 0;
        SEG_COM_GRID[E8_LCD[0]][E8_LCD[1]] = 0;
        SEG_COM_GRID[F8_LCD[0]][F8_LCD[1]] = 0;
        SEG_COM_GRID[G8_LCD[0]][G8_LCD[1]] = 0;
        break;
    case 9:
        SEG_COM_GRID[A9_LCD[0]][A9_LCD[1]] = 0;
        SEG_COM_GRID[B9_LCD[0]][B9_LCD[1]] = 0;
        SEG_COM_GRID[C9_LCD[0]][C9_LCD[1]] = 0;
        SEG_COM_GRID[D9_LCD[0]][D9_LCD[1]] = 0;
        SEG_COM_GRID[E9_LCD[0]][E9_LCD[1]] = 0;
        SEG_COM_GRID[F9_LCD[0]][F9_LCD[1]] = 0;
        SEG_COM_GRID[G9_LCD[0]][G9_LCD[1]] = 0;
        break;
    case 10:
        SEG_COM_GRID[A10_LCD[0]][A10_LCD[1]] = 0;
        SEG_COM_GRID[B10_LCD[0]][B10_LCD[1]] = 0;
        SEG_COM_GRID[C10_LCD[0]][C10_LCD[1]] = 0;
        SEG_COM_GRID[D10_LCD[0]][D10_LCD[1]] = 0;
        SEG_COM_GRID[E10_LCD[0]][E10_LCD[1]] = 0;
        SEG_COM_GRID[F10_LCD[0]][F10_LCD[1]] = 0;
        SEG_COM_GRID[G10_LCD[0]][G10_LCD[1]] = 0;
        break;
    case 11:
        SEG_COM_GRID[A11_LCD[0]][A11_LCD[1]] = 0;
        SEG_COM_GRID[B11_LCD[0]][B11_LCD[1]] = 0;
        SEG_COM_GRID[C11_LCD[0]][C11_LCD[1]] = 0;
        SEG_COM_GRID[D11_LCD[0]][D11_LCD[1]] = 0;
        SEG_COM_GRID[E11_LCD[0]][E11_LCD[1]] = 0;
        SEG_COM_GRID[F11_LCD[0]][F11_LCD[1]] = 0;
        SEG_COM_GRID[G11_LCD[0]][G11_LCD[1]] = 0;
        break;
    }
}

void GF90970_set_digit(int digit_num, int value) {

    static void (*_map[])(int digit) = {
        GF90970_number_0,
        GF90970_number_1,
        GF90970_number_2,
        GF90970_number_3,
        GF90970_number_4,
        GF90970_number_5,
        GF90970_number_6,
        GF90970_number_7,
        GF90970_number_8,
        GF90970_number_9,
    };

    GF90970_clear_digit(digit_num);

    _map[value](digit_num);
}

#define NUM_ANIM_SEGMENTS 12

static int *segments[] = {
    A1_LCD, A2_LCD, A3_LCD, A4_LCD,
    B4_LCD, C4_LCD,
    D4_LCD, D3_LCD, D2_LCD, D1_LCD,
    E1_LCD, F1_LCD,
};

void GF90970_boot_anim_step() {

    static int idx;
    static bool wifi_on;

    int first_seg = idx % NUM_ANIM_SEGMENTS;
    int second_seg = (idx + 1) % NUM_ANIM_SEGMENTS;

    GF90970_clear_digit(1);
    GF90970_clear_digit(2);
    GF90970_clear_digit(3);
    GF90970_clear_digit(4);

    SEG_COM_GRID[segments[first_seg][0]][segments[first_seg][1]] = 1;
    SEG_COM_GRID[segments[second_seg][0]][segments[second_seg][1]] = 1;

    if (idx % 4 == 0) {
        wifi_on = !wifi_on;
    }

    if (wifi_on) {
        GF90970_wifi_detected_on();
    } else {
        GF90970_wifi_detected_off();
    }

    idx++;
}

void GF90970_ap_reset(void) {
    
    GF90970_clear_digit(8);
    GF90970_clear_digit(9);
    GF90970_clear_digit(10);
    GF90970_clear_digit(11);

    SEG_COM_GRID[A9_LCD[0]][A9_LCD[1]] = 1;
    SEG_COM_GRID[B9_LCD[0]][B9_LCD[1]] = 1;
    SEG_COM_GRID[C9_LCD[0]][C9_LCD[1]] = 1;
    SEG_COM_GRID[D9_LCD[0]][D9_LCD[1]] = 0;
    SEG_COM_GRID[E9_LCD[0]][E9_LCD[1]] = 1;
    SEG_COM_GRID[F9_LCD[0]][F9_LCD[1]] = 1;
    SEG_COM_GRID[G9_LCD[0]][G9_LCD[1]] = 1;

    SEG_COM_GRID[A10_LCD[0]][A10_LCD[1]] = 1;
    SEG_COM_GRID[B10_LCD[0]][B10_LCD[1]] = 1;
    SEG_COM_GRID[C10_LCD[0]][C10_LCD[1]] = 0;
    SEG_COM_GRID[D10_LCD[0]][D10_LCD[1]] = 0;
    SEG_COM_GRID[E10_LCD[0]][E10_LCD[1]] = 1;
    SEG_COM_GRID[F10_LCD[0]][F10_LCD[1]] = 1;
    SEG_COM_GRID[G10_LCD[0]][G10_LCD[1]] = 1;
}

void GF90970_presets_reset(void) {

    GF90970_clear_digit(5);
    GF90970_clear_digit(6);

    SEG_COM_GRID[A5_LCD[0]][A5_LCD[1]] = 0;
    SEG_COM_GRID[B5_LCD[0]][B5_LCD[1]] = 1;
    SEG_COM_GRID[C5_LCD[0]][C5_LCD[1]] = 1;
    SEG_COM_GRID[D5_LCD[0]][D5_LCD[1]] = 1;
    SEG_COM_GRID[E5_LCD[0]][E5_LCD[1]] = 1;
    SEG_COM_GRID[F5_LCD[0]][F5_LCD[1]] = 0;
    SEG_COM_GRID[G5_LCD[0]][G5_LCD[1]] = 1;

    SEG_COM_GRID[A6_LCD[0]][A6_LCD[1]] = 0;
    SEG_COM_GRID[B6_LCD[0]][B6_LCD[1]] = 0;
    SEG_COM_GRID[C6_LCD[0]][C6_LCD[1]] = 0;
    SEG_COM_GRID[D6_LCD[0]][D6_LCD[1]] = 1;
    SEG_COM_GRID[E6_LCD[0]][E6_LCD[1]] = 1;
    SEG_COM_GRID[F6_LCD[0]][F6_LCD[1]] = 1;
    SEG_COM_GRID[G6_LCD[0]][G6_LCD[1]] = 0;
}

void GF90970_set_boot_stage_0(void) {

    GF90970_clear_digit(1);
    GF90970_clear_digit(2);
    GF90970_clear_digit(3);
    GF90970_clear_digit(4);

    SEG_COM_GRID[A1_LCD[0]][A1_LCD[1]] = 1;
    SEG_COM_GRID[B1_LCD[0]][B1_LCD[1]] = 0;
    SEG_COM_GRID[C1_LCD[0]][C1_LCD[1]] = 0;
    SEG_COM_GRID[D1_LCD[0]][D1_LCD[1]] = 1;
    SEG_COM_GRID[E1_LCD[0]][E1_LCD[1]] = 1;
    SEG_COM_GRID[F1_LCD[0]][F1_LCD[1]] = 1;
    SEG_COM_GRID[G1_LCD[0]][G1_LCD[1]] = 0;

    SEG_COM_GRID[A2_LCD[0]][A2_LCD[1]] = 1;
    SEG_COM_GRID[B2_LCD[0]][B2_LCD[1]] = 0;
    SEG_COM_GRID[C2_LCD[0]][C2_LCD[1]] = 0;
    SEG_COM_GRID[D2_LCD[0]][D2_LCD[1]] = 1;
    SEG_COM_GRID[E2_LCD[0]][E2_LCD[1]] = 0;
    SEG_COM_GRID[F2_LCD[0]][F2_LCD[1]] = 0;
    SEG_COM_GRID[G2_LCD[0]][G2_LCD[1]] = 0;

    SEG_COM_GRID[A3_LCD[0]][A3_LCD[1]] = 1;
    SEG_COM_GRID[B3_LCD[0]][B3_LCD[1]] = 0;
    SEG_COM_GRID[C3_LCD[0]][C3_LCD[1]] = 0;
    SEG_COM_GRID[D3_LCD[0]][D3_LCD[1]] = 1;
    SEG_COM_GRID[E3_LCD[0]][E3_LCD[1]] = 0;
    SEG_COM_GRID[F3_LCD[0]][F3_LCD[1]] = 0;
    SEG_COM_GRID[G3_LCD[0]][G3_LCD[1]] = 0;

    SEG_COM_GRID[A4_LCD[0]][A4_LCD[1]] = 1;
    SEG_COM_GRID[B4_LCD[0]][B4_LCD[1]] = 1;
    SEG_COM_GRID[C4_LCD[0]][C4_LCD[1]] = 1;
    SEG_COM_GRID[D4_LCD[0]][D4_LCD[1]] = 1;
    SEG_COM_GRID[E4_LCD[0]][E4_LCD[1]] = 0;
    SEG_COM_GRID[F4_LCD[0]][F4_LCD[1]] = 0;
    SEG_COM_GRID[G4_LCD[0]][G4_LCD[1]] = 0;
}

void GF90970_sync() {
    // send out in order: D0, D1, D2, D3 ==> COM0 bit, COM1 bit, COM2 bit, COM3 bit ==> each COM #'s bit is in corresponding column index

    // Create the 21 Combined int of 4 data bits
    int data_bits_grid[21] = {0};
    int i;

    for (i = 0; i < 21; i++) {
        int temp_data_bits[4] = {0};
        int k;

        for (k = 0; k < 4; k++) {
            // each iteration do want SEG_COM_GRID[i][j], but for each j, want shifted to left but not want other bits screwed up
            // so maybe have to just bit shift the 4 bits to their own shifted int's then OR them all together outside of this inner for loop?
            temp_data_bits[k] = (0x01 & SEG_COM_GRID[i][k]) << k;
        }
        data_bits_grid[i] = temp_data_bits[3] | temp_data_bits[2] | temp_data_bits[1] | temp_data_bits[0];
    }


    // Address is 0 (starts there), data bits array is data_bits_grid, total sets of data bits to send is 21
    write_data_consecutive(0, data_bits_grid, 21);
}
