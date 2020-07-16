#ifndef _STATE_H
#define _STATE_H

#include <stdio.h>
#include "lcd.h"
#include "touch.h"

#define EXIT -1 //退出系统
//父界面	 0~99
#define MAIN_FACE 0			//欢迎界面
#define MAIN_CONTROL 1 		//主控制界面
#define ALTER_CONTROL 2		//报警控制界面
#define DOOR_WINDOW 3		// 门窗监控界面
#define VIDEO_CENTOR 4		//视频中心
#define LIGHT_CONTROL 5		//led控制
#define DIGITAL_ALBUM 6     //数码相册
#define BGM	7				//背景音乐设置界面
#define PLAY_MUSIC 8		//播放音乐
#define KEY_TEST 9			//按键测试--->按键设置

//门窗监控的子界面 100~ 149
#define INFOR 100			//只选中通知设置
#define BUZZER 101			//只选中蜂鸣器设置
#define INFOR_BUZZER 102 		//通知设置和蜂鸣器设置都选中

//播放音乐的子界面 150 ~199
#define PLAYING 150			//播放中界面
#define STOP 151			//暂停界面



extern unsigned int state;	

void show(unsigned cur);				//根据界面状态显示对应的界面
void change_state(unsigned int sta);	//用于改变的状态,改为sta
//unsigned getState(unsigned cur, unsigned x, unsigned y);//获取对应坐标的状态
//bool site_isChangeState(unsigned cur, unsigned x, unsigned y);	//判断坐标是否是切换界面事件










#endif