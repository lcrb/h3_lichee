#ifndef __BUTTONS_H
#define __BUTTONS_H

void buttons_run(void);
void buttons_cleanup(void);

void buttons_setup_sysfs(void);

int buttons_check_test_mode(void);

#endif // __BUTTONS_H
