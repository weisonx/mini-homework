//头文件---内核源码
//GPIOC14 ---- key
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>

#include "cfg_type.h"

struct key_info {
	unsigned int gpio_num;
	char gpio_name[10];
};

static struct key_info gec6818_key[4] = {
	{
		.gpio_num = PAD_GPIO_A + 28,
		.gpio_name = "key2",
	},
	{
		.gpio_num = PAD_GPIO_B + 30,
		.gpio_name = "key3",
	},
	{
		.gpio_num = PAD_GPIO_B + 31,
		.gpio_name = "key4",
	},	
	{
		.gpio_num = PAD_GPIO_B + 9,
		.gpio_name = "key6",
	}	
};

static int gec6818_key_open(struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "key drv openning\n");
	return 0;
}

//是给应用程序的read()做系统调用的接口，用来读取驱动程序的按键数据
//struct file *filp ---- 指向驱动文件的指针
//const char __user *buf---应用程序读取数据的位置。数据是4个字节，每个字节代表一个按键的状态
//size_t len ---- 应用程序读取数据的字节数，4个字节
//loff_t *off ---文件指针的偏移量
static ssize_t gec6818_key_read(struct file *filp, char __user *buf, 
                             size_t len, loff_t *off)
{
	int ret,i;
	char key_flag[4]; //1--没有按下；0---按键按下
	if(len != 4)
		return -EINVAL;
	for(i=0;i<4;i++)
		key_flag[i] = gpio_get_value(gec6818_key[i].gpio_num);//获取按键的输入值
	ret = copy_to_user(buf, key_flag, len);
	if(ret != 0)
		return -EFAULT;

	return len;	
}

static int gec6818_key_release(struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "key drv closing\n");
	return 0;
}
//2.定义并初始化一个文件操作集
static struct file_operations gec6818_key_fops = {
	.owner = THIS_MODULE,
	.open = gec6818_key_open,
	.read = gec6818_key_read,
	.release = gec6818_key_release,
};


//3.定义并初始化一个混杂设备
static struct miscdevice gec6818_key_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "key_drv",
	.fops = &gec6818_key_fops,
};


//1.设计一个module
static int __init gec6818_key_init(void) //驱动的安装函数
{
	int ret,i;
	for(i=0;i<4;i++){
		ret = gpio_request(gec6818_key[i].gpio_num, gec6818_key[i].gpio_name); //申请4个按键的GPIO
		if(ret < 0){
			printk(KERN_WARNING "gpio%d request failed \n", i);
			goto gpio_request_error;
		}
		gpio_direction_input(gec6818_key[i].gpio_num);
	}
	
	//4.注册混杂设备
	ret = misc_register(&gec6818_key_misc);
	if(ret < 0){
		printk(KERN_WARNING "misc reg faikey\n");
		goto misc_reg_error;
	}
	
	printk(KERN_WARNING "gec6818 key init\n");
	return 0;
		
misc_reg_error:	
gpio_request_error:
	while(i--)//--i
		gpio_free(gec6818_key[i].gpio_num);		

	return ret;
}

static void __exit gec6818_key_exit(void) //驱动的卸载函数
{
	//释放内核资源，并注销驱动
	int i;
	for(i=0;i<4;i++)
		gpio_free(gec6818_key[i].gpio_num);	
	misc_deregister(&gec6818_key_misc);
	printk(KERN_WARNING "gec6818 key exit\n");
}

//#insmod key_drv.ko-->module_init()-->gec6818_key_init()
module_init(gec6818_key_init);//module的入口
//#rmmod key_drv.ko-->module_exit()-->gec6818_key_exit()
module_exit(gec6818_key_exit);//module的出口


//module的描述,不是必需的。#modinfo key_drv.ko
MODULE_AUTHOR("bobeyfeng@163.com");
MODULE_DESCRIPTION("GEC6818 KEY Device Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");
