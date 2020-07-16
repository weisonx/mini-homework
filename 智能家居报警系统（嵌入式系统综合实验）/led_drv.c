//头文件---内核源码
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/uaccess.h>

static unsigned int led_major = 200;
static unsigned int led_minor = 0;
static dev_t led_dev_num;

static struct class *led_class = NULL;
static struct device *gec6818_led_device = NULL;

static struct resource *gpioe_led_res = NULL;
static struct resource *gpioc_led_res = NULL;

//定义物理地址对应的虚拟地址的指针
static volatile unsigned int __iomem *GPIOE_BASE = NULL;
static volatile unsigned int __iomem *GPIOEOUT = NULL;     //0x00
static volatile unsigned int __iomem *GPIOEOUTENB = NULL;  //0x04
static volatile unsigned int __iomem *GPIOEALTFN0 = NULL;  //0x20

static volatile unsigned int __iomem *GPIOEPAD = NULL;	   //0x18 

static volatile unsigned int __iomem *GPIOC_BASE = NULL;
static volatile unsigned int __iomem *GPIOCOUT = NULL;     //0x00
static volatile unsigned int __iomem *GPIOCOUTENB = NULL;  //0x04
static volatile unsigned int __iomem *GPIOCALTFN0 = NULL;  //0x20
static volatile unsigned int __iomem *GPIOCALTFN1 = NULL;  //0x24

static volatile unsigned int __iomem *GPIOCPAD = NULL;	   //0x18

//2.创建一个cdev
static struct cdev gec6818_led_dev;

static int gec6818_led_open(struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "led drv openning\n");
	return 0;
}

//是给应用程序的write()做系统调用的接口，用来接收应用程序写下来的数据
//struct file *filp ---- 指向驱动文件的指针
//const char __user *buf---应用程序写下来的数据存放的位置
//size_t len ---- 应用程序写下来数据的字节数
//loff_t *off ---文件指针的偏移量
static ssize_t gec6818_led_write(struct file *filp, const char __user *buf, 
                             size_t len, loff_t *off)
{
	int ret;
	char led_flag[2];
	if(len != 2)
		return -EINVAL;
	ret = copy_from_user(led_flag, buf,len);
	if(ret != 0)
		return -EFAULT;
	printk(KERN_WARNING "led_flag[1]=%d,led_flag[0]=%d\n",
				led_flag[1],led_flag[0]);
	//用led_flag[2]配置寄存器
	switch(led_flag[1]){
	/*按键**************************************************/  //应该是要read函数？？
	case 0 ://按键0
		if(led_flag[0] == 1)//收到按键按下命令
			*GPIOEPAD 
	case 1 : //按键1
	
	case 2 : //按键2
	
	case 3 : //按键3
	
	/***************************************************/
	/*LED灯************************************************/
	case 7 : //D7
		if(led_flag[0] == 1) //D7 亮
			*GPIOEOUT &= ~(1<<13);
		else if(led_flag[0] == 0) //D7 灭
			*GPIOEOUT |= (1<<13);
		else
			return -EINVAL;
		break;
	case 8 : //D8
		if(led_flag[0] == 1) //D8 亮
			*GPIOCOUT &= ~(1<<17);
		else if(led_flag[0] == 0) //D8 灭
			*GPIOCOUT |= (1<<17);
		else
			return -EINVAL;
		break;
	case 9 : //D9
		if(led_flag[0] == 1) //D9 亮
			*GPIOCOUT &= ~(1<<8);
		else if(led_flag[0] == 0) //D9 灭
			*GPIOCOUT |= (1<<8);
		else
			return -EINVAL;
		break;	
	case 10 : //D10
		if(led_flag[0] == 1) //D10 亮
			*GPIOCOUT &= ~(1<<7);
		else if(led_flag[0] == 0) //D10 灭
			*GPIOCOUT |= (1<<7);
		else
			return -EINVAL;
		break;	
	/**************************************************/
	default:
		return -EINVAL;	
	}
	return len;	
}

static int gec6818_led_release(struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "led drv closing\n");
	return 0;
}
//4.定义并初始化一个文件操作集
static struct file_operations gec6818_led_fops = {
	.owner = THIS_MODULE,
	.open = gec6818_led_open,
	.write = gec6818_led_write,
	.release = gec6818_led_release,
};





//1.设计一个module
static int __init gec6818_led_init(void) //驱动的安装函数
{
	int ret;
	//3.申请设备号
	if(led_major != 0){
		led_dev_num = MKDEV(led_major, led_minor);
		ret = register_chrdev_region(led_dev_num, 1, "led_device");
	}else{
		ret = alloc_chrdev_region(&led_dev_num, led_minor, 1,
			"led_device");
	}
	if(ret < 0){
		printk(KERN_WARNING "can not get led num\n");
		return ret;		
	}
	
	//5.cdev初始化
	cdev_init(&gec6818_led_dev, &gec6818_led_fops);
	//6.将cdev加入内核
	ret = cdev_add(&gec6818_led_dev, led_dev_num, 1);
	if(ret < 0){
		printk(KERN_WARNING "cdev add failed\n");
		goto cdev_add_err;
	}
	//7.创建class
	led_class = class_create(THIS_MODULE, "gec6818_led"); // /sys/class/
	if (IS_ERR(led_class)){
		printk(KERN_WARNING "class create failed\n");
		ret = PTR_ERR(led_class);
		goto class_create_err;
	}
	//8.创建device
	gec6818_led_device = device_create(led_class, NULL,
						led_dev_num, NULL, "led_drv");// /dev/led_drv
	if (IS_ERR(gec6818_led_device)){
		printk(KERN_WARNING "device create failed\n");
		ret = PTR_ERR(gec6818_led_device);
		goto device_create_err;
	}
	
	//9.申请物理地址内存区作为一个资源(不是必需的)
	gpioe_led_res = request_mem_region(0xC001E000, 0x1000,   "GPIOE_MEM");// /proc/iomem
	if(gpioe_led_res == NULL){
		printk(KERN_WARNING "request gpioe mem failed\n");
		ret = -EBUSY;
		goto request_mem_gpioe_err;
	}
	gpioc_led_res = request_mem_region(0xC001C000, 0x1000,   "GPIOC_MEM");// /proc/iomem
	if(gpioe_led_res == NULL){
		printk(KERN_WARNING "request gpioc mem failed\n");
		ret = -EBUSY;
		goto request_mem_gpioc_err;
	}
	
	//10.IO内存动态映射，得到物理地址对应的虚拟地址
	GPIOE_BASE = ioremap(0xC001E000, 0x1000);
	if(GPIOE_BASE == NULL){
		printk(KERN_WARNING "gpioe ioremap failed\n");
		ret = -EFAULT;
		goto gpioe_ioremap_err;
	}
	GPIOEOUT = GPIOE_BASE + 0; //0x00
	GPIOEOUTENB = GPIOE_BASE + 1; //0x04
	GPIOEALTFN0 = GPIOE_BASE + 8; //0x20
	
	GPIOEPAD = GPIOE_BASE +	3;	  //0x18   //使用GPIOE的引脚作按键输入
	
	GPIOC_BASE = ioremap(0xC001C000, 0x1000);
	if(GPIOC_BASE == NULL){
		printk(KERN_WARNING "gpioe ioremap failed\n");
		ret = -EFAULT;
		goto gpioc_ioremap_err;
	}
	GPIOCOUT = GPIOC_BASE + 0; //0x00
	GPIOCOUTENB = GPIOC_BASE + 1; //0x04
	GPIOCALTFN0 = GPIOC_BASE + 8; //0x20	
	GPIOCALTFN1 = GPIOC_BASE + 9; //0x24
	
//	GPIOCPAD = GPIOC_BASE + 3; 	  //0x18
	
	//11.通过虚拟地址访问寄存器，来控制硬件
	//将GPIOE13配置成输出，输出高电平，D7灭
	*GPIOEALTFN0 &=  ~(3<<26);
	*GPIOEOUTENB |= (1<<13);
	*GPIOEOUT |=(1<<13);
	
	//将GPIOE 0，1，2，3配置成输入
	*GPIOEALTFN0 &= ~255;
	
	//将GPIOC17/C8/C7配置成输出，输出高电平，D8/9/10灭
	*GPIOCALTFN1 &= ~(3<<2);
	*GPIOCALTFN1 |= (1<<2);
	*GPIOCALTFN0 &= ~((3<<16) + (3<<14));
	*GPIOCALTFN0 |= ((1<<16) + (1<<14));
	*GPIOCOUTENB |= ((1<<17) + (1<<8) + (1<<7));
	*GPIOCOUT |= ((1<<17) + (1<<8) + (1<<7));
	
	printk(KERN_WARNING "gec6818 led init\n");
	return 0;
	
gpioc_ioremap_err:
	iounmap(GPIOE_BASE);
gpioe_ioremap_err:
	release_mem_region(0xC001C000, 0x1000);
request_mem_gpioc_err:
	release_mem_region(0xC001E000, 0x1000);	
request_mem_gpioe_err:
	device_destroy(led_class, led_dev_num);	
device_create_err:
	class_destroy(led_class);
class_create_err:	
	cdev_del(&gec6818_led_dev);
cdev_add_err:	
	unregister_chrdev_region(led_dev_num, 1);
	return ret;
}

static void __exit gec6818_led_exit(void) //驱动的卸载函数
{
	//释放内核资源，并注销驱动
	iounmap(GPIOC_BASE);
	iounmap(GPIOE_BASE);
	release_mem_region(0xC001C000, 0x1000);
	release_mem_region(0xC001E000, 0x1000);
	device_destroy(led_class, led_dev_num);
	class_destroy(led_class);
	cdev_del(&gec6818_led_dev);
	unregister_chrdev_region(led_dev_num, 1);
	printk(KERN_WARNING "gec6818 led exit\n");
}

//#insmod led_drv.ko-->module_init()-->gec6818_led_init()
module_init(gec6818_led_init);//module的入口
//#rmmod led_drv.ko-->module_exit()-->gec6818_led_exit()
module_exit(gec6818_led_exit);//module的出口


//module的描述,不是必需的。#modinfo led_drv.ko
MODULE_AUTHOR("bobeyfeng@163.com");
MODULE_DESCRIPTION("GEC6818 LED Device Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");
