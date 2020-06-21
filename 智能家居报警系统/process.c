#include "process.h"
#include "state.h"

extern unsigned int state;
void touch_event_process(unsigned cur, struct ts_xy ts)
{//分为改变状态（即改变界面）和工作事件（不需要改变界面）
	
	unsigned x = ts.x * 800 / 1024, y = ts.y * 480 / 600;	//分辨率是1024*600
	switch(cur)
	{
		case MAIN_FACE :
			if(x>282 && x<474 && y>309 && y<379)
				change_state(CONTROL);
				//state = CONTROL;
			break;

		case CONTROL :
			if(x >78 && x < 207 && y > 57 && y < 123)
				change_state(DOOR_WINDOW);
				//state = DOOR_WINDOW;
			else if(x >384 && x < 466 && y > 426 && y < 477)
				change_state(MAIN_FACE);	//返回主界面
				//state = MAIN_FACE;
			break;

		case DOOR_WINDOW :
			if(x >130 && x < 250 && y > 329 && y < 378)
				change_state(INFOR);
				//state = INFOR;
			else if(x >517 && x < 633 && y > 329 && y < 378)
				change_state(BEEP);
				//state = BEEP;
			else if(x >333 && x < 422 && y > 435 && y < 475)
				change_state(CONTROL);	//返回控制界面
				//state = CONTROL;
			break;

		case INFOR :
			change_state(DOOR_WINDOW);	//点击任意位置回到门窗控制界面
			//state = DOOR_WINDOW;
			break;
		case BEEP :
			change_state(DOOR_WINDOW);	//点击任意位置回到门窗控制界面
			//state = DOOR_WINDOW;
			break;
		case INFOR_BEEP :
			change_state(DOOR_WINDOW);	//点击任意位置回到门窗控制界面
			//state = DOOR_WINDOW;
			break;
		default :;
	}
	return;
}
