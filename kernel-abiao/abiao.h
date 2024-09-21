#ifndef _ABIAO_H_
#define _ABIAO_H_

#include <linux/cdev.h>
#include <linux/semaphore.h>

#define ABIAO_DEVICE_NODE_NAME "abiao"
#define ABIAO_DEVICE_FILE_NAME "abiao"
//#define ABIAO_DEVICE_PROC_NAME "abiao"
#define ABIAO_DEVICE_CLASS_NAME "abiao"

struct abiao_reg_dev {
	// 虚拟寄存器的值
	int val;
	// 信号量，多线程同步
	struct semaphore sem;
	// 标准linux字符结构体变量， 标志为字符设备
	struct cdev dev;
};

#endif
