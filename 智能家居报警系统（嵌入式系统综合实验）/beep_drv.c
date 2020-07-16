//头文件---内核源码
//GPIOC14 ---- beep
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


//static struct resource *gpioe_beep_res = NULL;


//定义物理地址对应的虚拟地址的指针
static volatile unsigned int __iomem *GPIOC_BASE = NULL;
static volatile unsigned int __iomem *GPIOCOUT = NULL;     //0x00
static volatile unsigned int __iomem *GPIOCOUTENB = NULL;  //0x04
static volatile unsigned int __iomem *GPIOCALTFN0 = NULL;  //0x20

static int gec6818_beep_open(struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "beep drv openning\n");
	return 0;
}

//是给应用程序的write()做系统调用的接口，用来接收应用程序写下来的数据
//struct file *filp ---- 指向驱动文件的指针
//const char __user *buf---应用程序写下来的数据存放的位置
//size_t len ---- 应用程序写下来数据的字节数
//loff_t *off ---文件指针的偏移量
static ssize_t gec6818_beep_write(struct file *filp, const char __user *buf, 
                             size_t len, loff_t *off)
{
	int ret;
	char beep_flag;
	if(len != 1)
		return -EINVAL;
	ret = copy_from_user(&beep_flag, buf,len);
	if(ret != 0)
		return -EFAULT;
	switch(beep_flag){
	case 1 : //蜂鸣器响
		*GPIOCOUT |= (1<<14);
		break;
		
	case 0 : //蜂鸣器不响
		*GPIOCOUT &= ~(1<<14);
		break;
	
	default:
		return -EINVAL;	
	}
	return len;	
}

static int gec6818_beep_release(struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "beep drv closing\n");
	return 0;
}
//2.定义并初始化一个文件操作集
static struct file_operations gec6818_beep_fops = {
	.owner = THIS_MODULE,
	.open = gec6818_beep_open,
	.write = gec6818_beep_write,
	.release = gec6818_beep_release,
};


//3.定义并初始化一个混杂设备
static struct miscdevice gec6818_beep_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "beep_drv",
	.fops = &gec6818_beep_fops,
};


//1.设计一个module
static int __init gec6818_beep_init(void) //驱动的安装函数
{
	int ret;
	//4.注册混杂设备
	ret = misc_register(&gec6818_beep_misc);
	if(ret < 0){
		printk(KERN_WARNING "misc reg faibeep\n");
		return ret;
	}
/*	
	//5.申请物理地址内存区作为一个资源(不是必需的)
	gpioc_beep_res = request_mem_region(0xC001C000, 0x1000,   "GPIOC_MEM");// /proc/iomem
	if(gpioe_beep_res == NULL){
		printk(KERN_WARNING "request gpioc mem faibeep\n");
		ret = -EBUSY;
		goto request_mem_gpioc_err;
	}
*/	
	//6.IO内存动态映射，得到物理地址对应的虚拟地址
	GPIOC_BASE = ioremap(0xC001C000, 0x1000);
	if(GPIOC_BASE == NULL){
		printk(KERN_WARNING "gpioe ioremap faibeep\n");
		ret = -EFAULT;
		goto gpioc_ioremap_err;
	}
	GPIOCOUT = GPIOC_BASE + 0; //0x00
	GPIOCOUTENB = GPIOC_BASE + 1; //0x04
	GPIOCALTFN0 = GPIOC_BASE + 8; //0x20	
	
	//7.通过虚拟地址访问寄存器，来控制硬件
	//将GPIOC14配置成输出，输出低电平，蜂鸣器不响
	*GPIOCALTFN0 &= ~(3<<28);
	*GPIOCALTFN0 |= (1<<28); //FN1--IO
	
	*GPIOCOUTENB |= (1<<14);
	*GPIOCOUT &= ~(1<<14);
	
	printk(KERN_WARNING "gec6818 beep init\n");
	return 0;
	

gpioc_ioremap_err:
//	release_mem_region(0xC001C000, 0x1000);
//request_mem_gpioc_err:
	misc_deregister(&gec6818_beep_misc);
	return ret;
}

static void __exit gec6818_beep_exit(void) //驱动的卸载函数
{
	//释放内核资源，并注销驱动
	iounmap(GPIOC_BASE);
	//release_mem_region(0xC001C000, 0x1000);
	misc_deregister(&gec6818_beep_misc);
	printk(KERN_WARNING "gec6818 beep exit\n");
}

//#insmod beep_drv.ko-->module_init()-->gec6818_beep_init()
module_init(gec6818_beep_init);//module的入口
//#rmmod beep_drv.ko-->module_exit()-->gec6818_beep_exit()
module_exit(gec6818_beep_exit);//module的出口


//module的描述,不是必需的。#modinfo beep_drv.ko
MODULE_AUTHOR("bobeyfeng@163.com");
MODULE_DESCRIPTION("GEC6818 BEEP Device Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");
