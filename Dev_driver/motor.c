#include <linux/module.h>  
#include <linux/kernel.h> //for printk  
#include <linux/cdev.h>       //for cdev_init, cdev_add..  
#include <linux/fs.h>     //alloc_chrdev_region..  
#include <asm/uaccess.h>  //copy_to_user.. 
#include <linux/ioport.h> //request_mem_region  
#include <linux/io.h>  
#include <linux/device.h> //for class, device  
#include <linux/gpio.h>       //for gpio 
#include <cfg_type.h>
#include <linux/timer.h>
//#include <stdio.h>
#include <linux/delay.h>
//定义电机脉冲和方向
#define Motor_Pul_C(x)  gpio_set_value(PAD_GPIO_C+4,x)  //c4
#define Motor_Pul_B(x)  gpio_set_value(PAD_GPIO_C+8,x)  //c8
#define Motor_Pul_A(x)  gpio_set_value(PAD_GPIO_C+28,x) //c28

#define Motor_Dir_C(x)  gpio_set_value(PAD_GPIO_C+7,x)  //c7
#define Motor_Dir_B(x)  gpio_set_value(PAD_GPIO_C+24,x) //c24
#define Motor_Dir_A(x)  gpio_set_value(PAD_GPIO_B+26,x) //b26

//定义限位开关信号
#define XianWei_C_S  gpio_get_value(PAD_GPIO_B+30)  
#define XianWei_B_S  gpio_get_value(PAD_GPIO_B+31) 
#define XianWei_A_S  gpio_get_value(PAD_GPIO_D+1) 

//定义高低电平
const int high = 1;
const int low = 0;

struct cdev cdev; //创建设备
dev_t devno; //设备号
static struct class *myclass ;
static struct device *myclass_dev ;

//文件读操作
static ssize_t dev_read(struct file *filp, char *buffer, size_t count, loff_t *ppos)
{
    int ret; 
    int result = 0;
    result = XianWei_A_S << 2 | XianWei_B_S << 1 | XianWei_C_S;

    ret = copy_to_user(buffer, (int *)&result, sizeof(result)); 
    if(ret>0)
    {
        return ret;
    }
    printk("\nread err\n");
    return sizeof(result);
}

//文件打开操作
static ssize_t  dev_open(struct inode *node, struct file *filp)

{	
	gpio_direction_output(PAD_GPIO_C+4,0);  //c4
	gpio_direction_output(PAD_GPIO_C+8,0);  //c
	gpio_direction_output(PAD_GPIO_C+28,0); //c28

	gpio_direction_output(PAD_GPIO_C+7,0);  //c7
	gpio_direction_output(PAD_GPIO_C+24,0); //c24
	gpio_direction_output(PAD_GPIO_B+26,0); //b26

	//定义限位开关信号
	gpio_direction_input(PAD_GPIO_B+30); 
	gpio_direction_input(PAD_GPIO_B+31) ;
	gpio_direction_input(PAD_GPIO_D+1) ;


	printk("<0> \n Motor dev_driver open\n");

	return 0;
}
static long dev_ioctl(struct file *file, unsigned int cmd,unsigned long arg)
{
	switch(cmd)
	{
		case 5:
 		switch(arg)
 		{
 			case 1:
			Motor_Pul_A(1);
			udelay(2);
    			Motor_Pul_A(0);
				break;
			case 2:
			Motor_Pul_B(1);
			udelay(2);
			Motor_Pul_B(0);
	                	break;
			case 3:
			Motor_Pul_C(1);
			udelay(2);
			Motor_Pul_C(0);
	                	break;
			default : 
				break;

 		}
			break;

case 0:
    	Motor_Dir_A(arg);	
	break;
case 1:
    	Motor_Dir_B(arg);	
	break;
case 4:
    	Motor_Dir_C(arg);	
	break;
case 3:	
	Motor_Pul_A(1);	
	Motor_Pul_B(1);	
	Motor_Pul_C(1);		
	udelay(10);
	Motor_Pul_A(0);
	Motor_Pul_B(0);
	Motor_Pul_C(0);
	break;
default : 
	break;
	}
	return 0;
}
//文件操作集
static struct file_operations dev_fops =

{
    .open = dev_open,
    .read = dev_read,
    .unlocked_ioctl= dev_ioctl,
    .owner= THIS_MODULE,
};

//驱动初始化
static int dev_init(void)  
{  	

//  unsigned long delay = jiffies + HZ/500;   
   /*注册设备+创建设备文件*/
    cdev_init(&cdev,&dev_fops);
    alloc_chrdev_region(&devno, 0 , 1 , "motor");
    cdev_add(&cdev, devno, 1);

    myclass = class_create(THIS_MODULE,"motor");
    myclass_dev = device_create(myclass,NULL,devno,0,"motor");

    printk("<0>\n 欢迎使用motor_contral! \n"); 
return 0;
}  

//驱动注销
static void dev_exit(void)  
{
	cdev_del(&cdev);
	unregister_chrdev_region(devno,1);
	device_unregister(myclass_dev);
	class_destroy(myclass);
	gpio_free(PAD_GPIO_C+4);
    printk("<0> \n Goodbye!\n");  
}  
 


module_init(dev_init);  
module_exit(dev_exit);  
  

MODULE_LICENSE("GPL");  
MODULE_DESCRIPTION("motor test");
