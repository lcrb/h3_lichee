#ifndef __GF90970_H
#define __GF90970_H

void GF90970_init(void);
void GF90970_set_boot_stage_0(void);
void GF90970_reinit(void);
void GF90970_cleanup(void);

void GF90970_clear(void);

// write digits in memory
void GF90970_number_1(int digit_num);
void GF90970_number_2(int digit_num);
void GF90970_number_3(int digit_num);
void GF90970_number_4(int digit_num);
void GF90970_number_5(int digit_num);
void GF90970_number_6(int digit_num);
void GF90970_number_7(int digit_num);
void GF90970_number_8(int digit_num);
void GF90970_number_9(int digit_num);
void GF90970_clear_digit(int digit_num);

void GF90970_set_digit(int digit_num, int value);

// write custom elements in memory
void GF90970_am_on(void);
void GF90970_am_off(void);
void GF90970_pm_on(void);
void GF90970_pm_off(void);
void GF90970_temp_units_c(void);
void GF90970_temp_units_f(void);
void GF90970_temp_units_off(void);
void GF90970_temp_over_ninety_nine_on(void);
void GF90970_temp_over_ninety_nine_off(void);
void GF90970_bt_detected_on(void);
void GF90970_bt_detected_off(void);
void GF90970_main_time_div_on(void);
void GF90970_main_time_div_off(void);
void GF90970_alarm_set_on(void);
void GF90970_alarm_set_off(void);
void GF90970_nap_on(void);
void GF90970_nap_off(void);
void GF90970_snooze_on(void);
void GF90970_snooze_off(void);
void GF90970_alarm_time_div_on(void);
void GF90970_alarm_time_div_off(void);
void GF90970_wifi_detected_on(void);
void GF90970_wifi_detected_off(void);

// write memory to screen
void GF90970_sync(void);

void GF90970_boot_anim_step(void);
void GF90970_ap_reset(void);
void GF90970_presets_reset(void);

#endif // __gf90970_h
