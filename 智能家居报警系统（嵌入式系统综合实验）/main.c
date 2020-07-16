#include <stdio.h>
#include "process.h"
#include "state.h"

unsigned int state;	//全局变量 当前界面的状态面状态，每一个状态对应一个界面

int main(void)
{
	state = MAIN_FACE;	//欢迎界面
	show(state);

	unsigned int temp_sta = state;	//保存state的临时变量
	struct ts_xy ts;   

	while(1)
	{	
		if(state != temp_sta)	//判断状态是否调用show显示新的界面
			show(state);	//显示界面和处理
		else if(state == EXIT)
			break;	//退出程序
		ts = ts_get_xy();	//获取坐标,不需要多线程
		temp_sta = state;	//可能改版state之前保存
		touch_event_process(state, ts);	//触摸事件，不会直接改变界面(辅助界面除外），而是改变state在下一次迭代改变界面
										//辅助界面用于如闪烁等的短暂性功能，直接调用lcd_show（）
	}

	return 0;
}