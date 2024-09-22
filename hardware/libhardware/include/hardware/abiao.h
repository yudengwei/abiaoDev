//
// Created by abiao on 24-9-21.
//

#ifndef ABIAO_H
#define ABIAO_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <hardware/hardware.h>
#include <hardware/hw_auth_token.h>

#define MODULE_HARDWARE_MODULE_ID "abiao"

typedef struct abiao_module {
    struct hw_module_t common;
} abiao_module_t;

typedef struct abiao_device {
     struct hw_device_t common;
     int fd;
     int (*write_string)(struct abiao_device *dev, const char *str);
     int (*read_string)(struct abiao_device *dev, char *str);
} abiao_device_t;

#endif //ABIAO_H
