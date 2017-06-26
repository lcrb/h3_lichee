#ifndef __SYSFS_UTILS_H
#define __SYSFS_UTILS_H

#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/bug.h>

static void sysfs_null_cb(void) {}

#define SYSFS_NULL sysfs_null_cb

#define SYSFS_GENERATE(name, int_value, min_value, max_value, permissions, __func, __read_func) \
static ssize_t __used name##_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) { \
    int ret; \
    ret = sprintf(buf, "%d\n", (int_value)); \
    if (__read_func != SYSFS_NULL) { __read_func(); } \
    \
    return ret; \
} \
static ssize_t __used name##_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) { \
    int val; \
    sscanf(buf, "%du", &val); \
    if (val < (min_value)) val = min_value; \
    if (val > (max_value)) val = max_value; \
    int_value = val; \
    if (__func != SYSFS_NULL) __func(); \
    return count; \
} \
static struct kobj_attribute name##_attribute =__ATTR(name, permissions, name##_show, name##_store);

#define SYSFS_CREATE_FILE(kobj, name) { \
    int error; \
    error = sysfs_create_file((kobj), &(name##_attribute.attr)); \
    BUG_ON(error != 0); \
}

#endif // __SYSFS_UTILS_H
