#ifndef _EVENTS_H
#define _EVENTS_H
//处理不需要切换界面的事件
#include <stdio.h>
extern unsigned int state;

int led_set();

int turn_on_buzzer(void);

void music_playing(void);
void music_stop(void);	
void music_pause(void);	
void music_continue(void);

int slide_show(void);

int key_test(int key_num);
void flash(void);

#endif