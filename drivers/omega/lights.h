#ifndef __LIGHTS_H
#define __LIGHTS_H

void lights_init(void);
void lights_cleanup(void);

void lights_setup_sysfs(void);
int lights_test_step(void);

#endif // __LIGHTS_H
