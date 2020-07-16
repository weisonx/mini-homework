#ifndef _PROCESS_H
#define _PROCESS_H
#include <stdio.h>
#include "touch.h"


void touch_event_process(unsigned cur, struct ts_xy ts);//根据界面状态处理不同的触摸事件

//各个界面的处理函数
void main_face_process(unsigned x, unsigned y);

void main_control_process(unsigned x, unsigned y);

void light_control_process(unsigned x, unsigned y);

void video_centor_proces(unsigned x, unsigned y);
void alter_control_process(unsigned x, unsigned y);
void door_windos_process(unsigned x, unsigned y);
void buzzer_set_process(unsigned x, unsigned y);

void BGM_process(unsigned x, unsigned y);
void play_music_process(unsigned x, unsigned y);
void playing_process(unsigned x, unsigned y);
void stop_process(unsigned x, unsigned y);

void digital_albun_process(void);

void key_test_process(unsigned x, unsigned y);
#endif