#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "work.h"
#include "getPosPhread.h"
#include "touch.h"
#include "lcd.h"
static struct ts_xy ts_stic;	//保存获得的触摸坐标的静态全局变量

int key_test(int key_num)
{	

	/******驱动实现********************************/
	int fd_key;
	char key[4]; 
	int ret, i = 10*1000000;
	
	fd_key = open("/dev/key_drv",O_RDONLY);
	if(fd_key == -1)
	{
		perror("open key drv");
		return -1;		
	}
	while(i--) {
		ret = read(fd_key, key, sizeof(key));
		if(ret == -1)
		{
			perror("write key drv");
			return -1;				
		}
		//for(i=0;i<4;i++)
			//printf("key[%d] = %d\n", i, key[i]);

		if(key[key_num] == 0)
			flash();	
	}	
	
	close(fd_key);
	return 0;
}
void flash(void)
{
	int i = 1000;
	while(i--)
	{
		lcd_show_bmp("test_key_flash.bmp");
		int j = 1000;
		lcd_show_bmp("test_key_control.bmp");
		while(j--);
			
	}
	lcd_show_bmp("test_key_control.bmp");
}

/**********************************/

int led_set()
{
	//led_num:7,8,9,10
	//led_state：0,1
	int fd_led;
	char led_buf[2];
	int ret,i;
	fd_led = open("/dev/led_drv",O_WRONLY);
	if(fd_led == -1)
	{
		perror("open led drv");
		return -1;		
	}
	for(int i = 0; i != 4; ++i) {	//全部LED灯亮
		led_buf[1] = i+7;
		led_buf[0] = 1;	//亮
		ret = write(fd_led, led_buf, sizeof(led_buf));
		if(ret == -1)
		{
			perror("write led drv");
			return -1;				
		}
	}
	close(fd_led);
	
	return 0;
}

int turn_on_buzzer(void)
{
	int fd_buz;
	char buz_buf[2];
	int ret,i;
	fd_buz = open("/dev/buz_drv",O_WRONLY);//蜂鸣器驱动
	if(fd_buz == -1)
	{
		perror("open buz drv");
		return -1;		
	}
	int n = 0;
	while(n < 2)
	{
		buz_buf[1] = 1;
		buz_buf[0]=1;
		ret = write(fd_buz, buz_buf, sizeof(buz_buf));//写入数据，播放蜂鸣器
		if(ret == -1)
		{
			perror("write buz drv");
			return -1;				
		}
		buz_buf[1] = 1;
		buz_buf[0]=0;
		usleep(500*1000);//0.5s
		ret = write(fd_buz, buz_buf, sizeof(buz_buf));
		if(ret == -1)
		{
			perror("write buz drv");
			return -1;				
		}
		usleep(500*1000);//0.5s
		if(i==10)
			i=6;

		++n;
	}

	
	close(fd_buz);
	return 0;
}

void music_playing(void)
{
	system("madplay Cloud9.mp3 -r &");	//新进程播放音乐
}
void music_stop(void)
{
	system("killall -KILL madplay &");	//杀死播放进行
}
void music_pause(void)
{
	system("killall -STOP madplay &");//阻塞播放进程， ”&“ ??
}
void music_continue(void)
{
	system("killall -CONT madplay &");	//激活播放进程

}


int slide_show(void)
{
	int ret = 0;
	pthread_t pt; /*定义一个线程的ID，typedef unsigned long int pthread_t */

	ret = pthread_create(&pt, NULL, (void*)start_routine, &ts_stic);	//建立子线程
	if(ret != 0)							/*建立线程失败*/
	{
		printf("建立线程失败\n");
		return -1;							/*主线程退出*/
	}	
	usleep(1);								/*睡眠等待1us*/

	while(1) 
	{
		int i = 2*1000000;
		if(ts_stic.x >680 && ts_stic.x <762 && ts_stic.y >430 && ts_stic.y < 447)
			break;
		else
		{
			lcd_show_bmp("image1.bmp");
			while(--i)
				if(ts_stic.x >680 && ts_stic.x <762 && ts_stic.y >430 && ts_stic.y < 447)
					return 0;
			i = 2*1000000;

			lcd_show_bmp("image2.bmp");
			while(--i)
				if(ts_stic.x >680 && ts_stic.x <762 && ts_stic.y >430 && ts_stic.y < 447)
					return 0;
			i = 2*1000000;

			lcd_show_bmp("image3.bmp");
			while(--i)
				if(ts_stic.x >680 && ts_stic.x <762 && ts_stic.y >430 && ts_stic.y < 447)
					return 0;
		}
	}
}