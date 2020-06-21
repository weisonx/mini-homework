#include <stdio.h>
#include "process.h"
#include "state.h"

unsigned int state;	//当前界面的状态面状态，每一个状态对应一个界面

int main(void)
{
	state = MAIN_FACE;	//欢迎界面
	struct ts_xy ts;	//坐标变量
	unsigned int temp_sta = state;
	show(state);
	while(1)
	{	
		if(state != temp_sta)	//改变了状态才调用show显示新的界面
			show(state);	//显示界面和处理
		ts = ts_get_xy();	//获取坐标
		temp_sta = state;	//可能改版state之前保存
		touch_event_process(state, ts);	//触摸事件，不会直接改变界面，而是改变state来间接改变界面
	}

	return 0;
}