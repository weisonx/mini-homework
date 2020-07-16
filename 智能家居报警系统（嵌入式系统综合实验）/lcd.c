#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
 
int lcd_show_bmp(char *bmp_name)
{
	int fd_bmp,fd_lcd;
	char bmp_buf[800*480*3];//RGB
	int lcd_buf[800*480];//ARGB
	int lcd_new[800*480]; //不超过8MB(stack)
	
	int *lcd_base = NULL; //指向映射后用户空间中显存的首地址，int *---lcd_base可以通过偏移量指向每个像素点。
	fd_bmp = open(bmp_name, O_RDONLY);
	if(fd_bmp == -1)
	{
		perror("open bmp");
		return -1;		
	}
	lseek(fd_bmp, 54, SEEK_SET);
	read(fd_bmp, bmp_buf, sizeof(bmp_buf)); //800*480*3
	close(fd_bmp);
	
	
	for(int i=0; i<800*480;i++)
		lcd_buf[i] = (0x00<<24) + (bmp_buf[3*i+2]<<16) + (bmp_buf[3*i+1]<<8) + bmp_buf[3*i];
	
	//图片反转
	for(int i=0;i<800;i++)
		for(int j=0;j<480;j++)
			lcd_new[i + j*800] = lcd_buf[i + (479-j)*800];
	
	
	
	fd_lcd = open("/dev/fb0", O_RDWR);
	if(fd_lcd == -1)
	{
		perror("open lcd");
		return -1;
	}
	//显存的映射
	lcd_base = mmap(NULL, 800*480*4,  PROT_READ|PROT_WRITE, MAP_SHARED,
                  fd_lcd, 0);  //heap
	if(lcd_base == NULL)
	{
		perror("mmap error");
		return -1;	
	}
	for(int i=0;i<800*480;i++)
		*(lcd_base+i) =  lcd_new[i]; //将数据写入显存
	
	//解除显存映射
	munmap(lcd_base, 800*480*4);	
	
	close(fd_lcd);
	
	return 0;
}


int lcd_show_color(unsigned int color)
{
	int fd_lcd;
	
	int lcd_buf[800*480];//ARGB
	int *lcd_base = NULL; 
	
	fd_lcd = open("/dev/fb0", O_RDWR);
	if(fd_lcd == -1)
	{
		perror("open lcd");
		return -1;
	}
	//显存的映射
	lcd_base = mmap(NULL, 800*480*4,  PROT_READ|PROT_WRITE, MAP_SHARED,
                  fd_lcd, 0);  //heap
	if(lcd_base == NULL)
	{
		perror("mmap error");
		return -1;	
	}
	
	for(int i=0;i<800*480;i++)
		*(lcd_base+i) =  color; //将数据写入显存
	
	//解除显存映射
	munmap(lcd_base, 800*480*4);	
	
	close(fd_lcd);
	
	return 0;
}