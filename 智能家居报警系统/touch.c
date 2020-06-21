#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/input.h>

#include "touch.h"

struct ts_xy ts_get_xy(void)
{
	int fd_ts;
	struct ts_xy ts_temp;
	struct input_event  ts_data;
	//1.打开驱动
	fd_ts = open("/dev/input/event0", O_RDONLY);
	if(fd_ts == -1)
	{
		perror("open ts");
	}
	for(int i=0; i<8; i++)
	{
		//2.读取触摸屏的数据
		read(fd_ts, &ts_data,  sizeof(struct input_event));
		
		//3.得到X和Y的坐标
		if(ts_data.type == EV_ABS)
		{
			if(ts_data.code == ABS_X)
				ts_temp.x = ts_data.value;
			else if(ts_data.code == ABS_Y)
				ts_temp.y = ts_data.value;			
		}
	}
	
	//4.关闭驱动
	close(fd_ts);

	
	return  ts_temp;
}