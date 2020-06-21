#ifndef _STATE_H
#define _STATE_H

#include "lcd.h"
#include "touch.h"

//父界面	 0~99
#define MAIN_FACE 0	//欢迎界面
#define CONTROL 1		//控制界面
#define DOOR_WINDOW 2 // 门窗监控界面

//门窗监控的子界面 100~ 199
#define INFOR 100		//只选中通知设置
#define BEEP 101		//只选中蜂鸣器设置
#define INFOR_BEEP 102 //通知设置和蜂鸣器设置都选中

extern unsigned int state;	

void show(unsigned cur);	//根据界面状态显示对应的界面
void change_state(unsigned int sta);	//用于改变的状态,改为sta
//unsigned getState(unsigned cur, unsigned x, unsigned y);//获取对应坐标的状态
//bool site_isChangeState(unsigned cur, unsigned x, unsigned y);	//判断坐标是否是切换界面事件















#endif