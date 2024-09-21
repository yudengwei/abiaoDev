//
// Created by abiao on 24-9-21.
//

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>

#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
 
#include <linux/poll.h>
#include <linux/seq_file.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
 
#include <linux/slab.h>

#include "abiao.h"

// 主设备号
static int abiao_major = 0;
// 从设备号
static int abiao_minor = 0;

static struct class* abiao_class = NULL;
static struct abiao_reg_dev* abiao_dev = NULL;

static int abiao_open(struct inode* inode, struct file* file);
static int abiao_release(struct inode* inode, struct file* file);
static ssize_t abiao_read(struct file* filp, char __user* buf, size_t count, loff_t* f_pos);
static ssize_t abiao_write(struct file* filp, const char __user* buf, size_t count, loff_t* f_pos);

static struct file_operations abiao_fops = {
	.owner = THIS_MODULE,
	.open = abiao_open,
	.release = abiao_release,
	.read = abiao_read,
	.write = abiao_write,
};

// 这两个函数用于处理 DEVICE_ATTR 这个宏定义的处理
// 这个宏主要是在 /sys/devices/virtual/ 目录下生成对应的文件，使得开发人员可以通过 cat和echo 来进行操作
// 可以参考 https://www.cnblogs.com/lifexy/p/9799778.html 了解详情
static ssize_t abiao_val_show(struct device* dev, struct device_attribute* attr, char* buf);
static ssize_t abiao_val_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count);

static DEVICE_ATTR(val, S_IRUGO|S_IWUSR, abiao_val_show, abiao_val_store);

static int abiao_open(struct inode* inode, struct file* file) {
	struct abiao_reg_dev* dev;
	dev = container_of(inode->i_cdev, struct abiao_reg_dev, dev);
	file->private_data = dev;
	return 0;
}

static int abiao_release(struct inode* inode, struct file* file) {
	return 0;
}

static ssize_t abiao_read(struct file* filp, char __user* buf, size_t count, loff_t* f_pos) {
	ssize_t err = 0;
	struct abiao_reg_dev* dev = filp->private_data;
	if (down_interruptible(&(dev->sem))) {
		return -ERESTARTSYS;
	}
	if (count < sizeof(dev->val)) {
		goto out;
	}
	if (copy_to_user(buf, &(dev->val), sizeof(dev->val))) {
		err = -EFAULT;
		goto out;
	}
	err = sizeof(dev->val);
out:
	up(&(dev->sem));
	return err;
}

static ssize_t abiao_write(struct file* filp, const char __user* buf, size_t count, loff_t* f_pos) {
	struct abiao_reg_dev* dev = filp->private_data;
	ssize_t err = 0;
	if (down_interruptible(&(dev->sem))) {
		return -ERESTARTSYS;
	}
	if (count < sizeof(dev->val)) {
		goto out;
	}
	if (copy_from_user(&(dev->val), buf, count)) {
		err = -EFAULT;
		goto out;
	}
	err = sizeof(dev->val);
out:
	up(&(dev->sem));
	return err;
}

static ssize_t __abiao_get_val(struct abiao_reg_dev* dev, char* buf) {
	int val = 0;
	if (down_interruptible(&(dev->sem))) {
		return -ERESTARTSYS;
	}
	val = dev->val;
	up(&(dev->sem));
	return snprintf(buf, PAGE_SIZE, "%d\n", val);
}

static ssize_t __abiao_set_val(struct abiao_reg_dev* dev, const char* buf, size_t count) {
	int val = 0;
	val = simple_strtol(buf, NULL, 0);
	if (down_interruptible(&(dev->sem))) {
		return -ERESTARTSYS;
	}
	dev->val = val;
	up(&(dev->sem));
	return count;
}

static ssize_t abiao_val_show(struct device* dev, struct device_attribute* attr, char* buf) {
	struct abiao_reg_dev* hdev = (struct abiao_reg_dev*)dev_get_drvdata(dev);
	return __abiao_get_val(hdev, buf);
}

static ssize_t abiao_val_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count) {
	struct abiao_reg_dev* hdev = (struct abiao_reg_dev*)dev_get_drvdata(dev);
	return __abiao_set_val(hdev, buf, count);
}

//static ssize_t abiao_proc_read(char* page, char** start, off_t off, int count, int* eof, void* data) {
//	if (off > 0) {
//		*eof = 1;
//		return 0;
//	}
//	return __abiao_get_val(abiao_dev, page);
//}
//
//static ssize_t abiao_proc_write(struct file* filp, const char __user* buff, unsigned long len, void* data) {
//	int err = 0;
//	char* page = NULL;
//	if (len > PAGE_SIZE) {
//		printk(KERN_ALERT"The buff is too large: %lu.\n", len);
//		return -EFAULT;
//	}
//	page = (char*)__get_free_page(GFP_KERNEL);
//	if (!page) {
//		printk(KERN_ALERT"Failed to alloc page.\n");
//		return -EFAULT;
//	}
//	if (copy_from_user(page, buff, len)) {
//		printk(KERN_ALERT"Failed to copy from user.\n");
//		err = -EFAULT;
//		goto out;
//	}
//	err = __abiao_set_val(abiao_dev, page, len);
//out:
//	free_page((unsigned long)page);
//	return err;
//}

//static void abiao_create_proc(void) {
//	struct proc_dir_entry* entry;
//	entry = create_proc_entry(ABIAO_DEVICE_PROC_NAME, 0, NULL);
//	if (entry) {
//		entry->owner = THIS_MODULE;
//		entry->read_proc = abiao_proc_read;
//		entry->write_proc = abiao_proc_write;
//	}
//}
//
//static void abiao_remove_proc(void) {
//	remove_proc_entry(ABIAO_DEVICE_PROC_NAME, NULL);
//}

//初始化dev
static int __abiao_setup_dev(struct abiao_reg_dev* dev) {
	int err;
	dev_t devno = MKDEV(abiao_major, abiao_minor);
	memset(dev, 0, sizeof(struct abiao_reg_dev));
	
	// 初始化字符设备
	cdev_init(&(dev->dev), &abiao_fops);
	dev->dev.owner = THIS_MODULE;
	dev->dev.ops = &abiao_fops;
	
	// 注册字符设备
	err = cdev_add(&(dev->dev), devno, 1);
	if (err) {
		return err;
	}
	
	//init_MUTEX(&(dev->sem)); 老的写法，新的不能用
	// 新写法 初始化信号量
	sema_init(&(dev->sem), 1);
	dev->val = 0;
	return 0;
}

static int __init hello_init(void){
    int err = -1;
    dev_t dev = 0;
    struct device* temp = NULL;
    printk(KERN_ALERT"Initializing abiao device.\n");

    // 动态分配主设备号和从设备号
    err = alloc_chrdev_region(&dev, 0, 1, ABIAO_DEVICE_NODE_NAME);
    if (err < 0) {
	    printk(KERN_ALERT"Failed to alloc char dev region.\n");
	    goto fail;
	}
	// 主设备号赋值
	abiao_major = MAJOR(dev);
	// 从设备号赋值
	abiao_minor = MINOR(dev);

	// 给dev分配内存
	abiao_dev = kmalloc(sizeof(struct abiao_reg_dev), GFP_KERNEL);
	if (!abiao_dev) {
		err = -ENOMEM;
		printk(KERN_ALERT"Failed to alloc abiao device.\n");
		goto unregister;
	}
	err = __abiao_setup_dev(abiao_dev);
	if (err) {
		printk(KERN_ALERT"Failed to setup abiao device: %d.\n", err);
		goto cleanup;
	}

	// 在/sys/class/ 目录下创建设备目录 abiao
	abiao_class = class_create(THIS_MODULE, ABIAO_DEVICE_CLASS_NAME);
	if (IS_ERR(abiao_class)) {
		err = PTR_ERR(abiao_class);
		printk(KERN_ALERT"Failed to create abiao devoce class.\n");
		goto destroy_cdev;
	}
	temp = device_create(abiao_class, NULL, dev, NULL, "%s", ABIAO_DEVICE_FILE_NAME);
	if (IS_ERR(temp)) {
		err = PTR_ERR(temp);
		printk(KERN_ALERT"Failed to create abiao device.\n");
		goto destroy_class;
	}
	err = device_create_file(temp, &dev_attr_val);
	if (err) {
		printk(KERN_ALERT"Failed to create attribute val of abiao debice.\n");
		goto destroy_device;
	}
	dev_set_drvdata(temp, abiao_dev);
	//abiao_create_proc();
	printk(KERN_ALERT"Successfully initialized abiao device.\n");
    return 0;
destroy_device:
	device_destroy(abiao_class, dev);
destroy_class:
	class_destroy(abiao_class);
destroy_cdev:
	cdev_del(&(abiao_dev->dev));
cleanup:
    kfree(abiao_dev);
unregister:
	unregister_chrdev_region(MKDEV(abiao_major, abiao_minor), 1);
fail:
	return err;
}


static void __exit hello_exit(void){
    dev_t devno = MKDEV(abiao_major, abiao_minor);
    printk(KERN_ALERT"Destroy abiao device.\n");
    //abiao_remove_proc();
    if (abiao_class) {
	    device_destroy(abiao_class, MKDEV(abiao_major, abiao_minor));
	    class_destroy(abiao_class);
	}
	if (abiao_dev) {
		cdev_del(&(abiao_dev->dev));
		kfree(abiao_dev);
	}
	unregister_chrdev_region(devno, 1);
    return;
}


module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
