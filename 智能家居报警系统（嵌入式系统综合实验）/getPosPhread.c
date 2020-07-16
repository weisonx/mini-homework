#include "getPosPhread.h"
#include "touch.h"
#include "stdio.h"

void *start_routine(void *arg)					
{
	struct ts_xy *ts_p = arg;		/*获取子线程创建的时候，传递给子线程的参数*/
	int flag = 1;
	while(flag)						
	{
		*ts_p = ts_get_xy();
		unsigned x = ts_p ->x * 800 / 1024, y = ts_p -> y * 480 / 600;	//分辨率是1024*600
		 ts_p -> x = x;
		 ts_p -> y = y;
		printf("坐标值 x = %d, y = %d\n",x, y);
		if(x >680 && x <762 && y >430 && y < 447)
			flag = 0;
	}
	printf("子线程退出\n");	

	int retvalue = 0;	/*设置退出值*/
	pthread_exit( (void*)&retvalue);/*线程退出并设置退出值*/
}
