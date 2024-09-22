//
// Created by abiao on 24-9-21.
//

#define LOG_TAG "ABiaoHAL"

#include <malloc.h>
#include <stdint.h>
#include <log/log.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cutils/atomic.h>
#include <stdlib.h>
#include <unistd.h>
#include <hardware/hardware.h>
#include <hardware/abiao.h>

#define DEVICE_NAME "/dev/abiao"
#define MODULE_NAME "Defaut Abiao HAL"
#define MODULE_AUTHOR "The Android Open Source Project"

static int abiao_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device);
static int abiao_device_close(struct hw_device_t* device);

static int abiao_write_string(struct abiao_device* dev, const char* str);
static int abiao_read_string(struct abiao_device* dev, char* str);

static struct hw_module_methods_t abiao_module_methods = {
    .open = abiao_device_open,
};

abiao_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = 1,
        .hal_api_version = 1,
        .id = MODULE_HARDWARE_MODULE_ID,
        .name = MODULE_NAME,
        .author = MODULE_AUTHOR,
        .methods = &abiao_module_methods,
    }
};

static int abiao_device_open(const struct hw_module_t* module, const char* name, struct hw_device_t** device) {
    abiao_device_t* dev = malloc(sizeof(abiao_device_t));
    memset(dev, 0, sizeof(abiao_device_t));

    ALOGE("Abiao: abiao_device_open name = %s", name);
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (hw_module_t*)module;
    dev->common.close = abiao_device_close;
    dev->write_string = abiao_write_string;
    dev->read_string = abiao_read_string;

    if ((dev->fd = open(DEVICE_NAME, O_RDWR)) == -1) {
        ALOGE("ABiao: open /dev/abiao fail----%s.", strerror(errno));
        free(dev);
        return -EFAULT;
    }

    // 将这个device赋值过去
    *device = &(dev->common);
    ALOGE("Abiao: open /dev/abiao successfully.");
    return 0;
}

static int abiao_device_close(struct hw_device_t* device) {
    struct abiao_device* abiao_device = (struct abiao_device*) device;
    if (abiao_device) {
      close(abiao_device->fd);
      free(abiao_device);
    }
    return 0;
}

static int abiao_write_string(struct abiao_device* dev, const char* str) {
    ALOGE("Abiao: write string: %s", str);
    write(dev->fd, str, sizeof(str));
    return 0;
}

static int abiao_read_string(struct abiao_device* dev, char* str) {
    ALOGE("Abiao: read string: ");
    read(dev->fd, str, sizeof(str));
    return 0;
}
