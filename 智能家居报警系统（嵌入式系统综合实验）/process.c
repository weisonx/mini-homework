#include <stdio.h>
#include "process.h"
#include "state.h"
#include "work.h"

extern unsigned int state;
void touch_event_process(unsigned cur, struct ts_xy ts)
{//分为改变状态（即改变界面）和工作事件（不需要改变界面）
	//if(site_isChangeState(cur, ts.x, ts.y))
	//	change_state(getState(cur, ts.x, ts.y));
	unsigned x = ts.x * 800 / 1024, y = ts.y * 480 / 600;	//分辨率是1024*600
	switch(cur)
	{
		/*主界面********************************************************/
		case MAIN_FACE :
			main_face_process(x,y);
			break;
		/***************************************************************/

		/*控制主界面*****************************************************/
		case MAIN_CONTROL :
			main_control_process(x,y);
			break;
		/***************************************************************/

		/*照明系统界面及子界面*******************************************/

		/**************************************************************/
		case LIGHT_CONTROL :
			light_control_process(x,y);
			break;

		/*报警系统界面及子界面*******************************************/
		case VIDEO_CENTOR:
			video_centor_proces(x,y);
			break;
		case ALTER_CONTROL :
			alter_control_process(x,y);
			break;

		case DOOR_WINDOW :
			door_windos_process(x,y);
			break;

		case INFOR :
			change_state(DOOR_WINDOW);	//点击任意位置回到门窗控制界面
			//state = DOOR_WINDOW;
			break;
		case BUZZER :
			buzzer_set_process(x,y);
			//change_state(DOOR_WINDOW);	//点击任意位置回到门窗控制界面
			//state = DOOR_WINDOW;
			break;
		//case INFOR_BUZZER :
		//	change_state(DOOR_WINDOW);	//点击任意位置回到门窗控制界面
		//	//state = DOOR_WINDOW;
		//	break;
		/***************************************************************/

		/*播放音乐界面及子界面********************************************/
		case BGM :
			BGM_process(x,y);
			break;
		case PLAY_MUSIC :
			play_music_process(x,y);
			break;
		case PLAYING :
			playing_process(x, y);
			break;
		case STOP :
			stop_process(x, y);
			break;
		/***************************************************************/

		/*数码相册界面及子界面********************************************/
		case DIGITAL_ALBUM :
			digital_albun_process();//因新建线程获取坐标，无须传坐标值
			break;
		/***************************************************************/

		/*按键测试****************************************************/
		case KEY_TEST :
			key_test_process(x,y);
			break;

		/************************************************************/
		default :;
	}
	return;
}

void main_face_process(unsigned x, unsigned y)
{
	if(x>282 && x<474 && y>309 && y<379)
		change_state(MAIN_CONTROL);
		//state = CONTROL;
}

void main_control_process(unsigned x, unsigned y)
{
	if(60<x && x <300 && 140<y && y < 200)
		change_state(LIGHT_CONTROL);
	else if(460<x && x < 700 && 140<y && y <200)
		change_state(ALTER_CONTROL);			//蜂鸣器
	else if(469<x && x <705 && 243 < y && y <304)
		change_state(DIGITAL_ALBUM);
	else if(63 < x && x < 300 && 352<y && y < 413)
		change_state(BGM);
	else if(460<x && x < 700 && y> 350 && y <410)
		change_state(KEY_TEST);
	else if(x > 700 && x < 800 && y > 440 && y < 480)
		change_state(EXIT);

}

void light_control_process(unsigned x, unsigned y)
{
	if(x > 730 && x <790 && y >370 && y < 400)
		led_set();
	else if(x >730 && x < 790 && y > 430 && y <460)
		change_state(MAIN_CONTROL);
}

void video_centor_proces(unsigned x, unsigned y)
{
	if(x >700 && x<780 && y >425 && y < 460)
		change_state(ALTER_CONTROL);
}

void alter_control_process(unsigned x, unsigned y)
{
	if(x >78 && x < 207 && y > 57 && y < 123)
		change_state(DOOR_WINDOW);
		//state = DOOR_WINDOW;
	else if(x >384 && x < 466 && y > 426 && y < 477)
		change_state(MAIN_FACE);	//返回主界面
		//state = MAIN_FACE;
	else if(x >350 && x < 510 && y > 180 && y < 290)
		change_state(VIDEO_CENTOR);
}

void door_windos_process(unsigned x, unsigned y)
{
	if(x >130 && x < 250 && y > 329 && y < 378)
		change_state(INFOR);
		//state = INFOR;
	else if(x >517 && x < 633 && y > 329 && y < 378)
		change_state(BUZZER);
		//state = BUZZER;
	else if(x >333 && x < 422 && y > 435 && y < 475)
		change_state(ALTER_CONTROL);	//返回控制界面
		//state = CONTROL;
}

void buzzer_set_process(unsigned x, unsigned y)
{
	if(x > 700 && x < 772 && y > 278 && y < 326)
		if(turn_on_buzzer() == -1)	//蜂鸣器响1秒左右
			printf("播方蜂鸣器失败\n");
	else if(x >130 && x < 250 && y > 329 && y < 378)
		change_state(INFOR);
	else
		change_state(DOOR_WINDOW);	//返回控制界面
	return;
}

void BGM_process(unsigned x, unsigned y)
{
	if(x > 630 && x < 760 && y > 170 && y < 220)
		change_state(PLAY_MUSIC);
	else if(x > 710 && x < 780 && y > 370 && y < 400)
		change_state(MAIN_CONTROL);
}

void play_music_process(unsigned x, unsigned y)	//在PYLA_MUSIC状态时是“播放”音乐
{
	if(x > 780 && x <800 && y > 0 && y < 16)	//关闭页面
	{	
		change_state(MAIN_CONTROL);
		music_stop();	//停止播放
		
	}
	else if(x > 520 && x < 574 && y > 193 && y < 245)
	{
		change_state(PLAYING);
		music_playing();//播放音乐
	}
}

void playing_process(unsigned x, unsigned y)
{
	if(x > 780 && x <800 && y > 0 && y < 16)	//关闭页面
	{	
		change_state(MAIN_CONTROL);
		music_stop();	//停止播放
		
	}
	else if(x > 520 && x < 574 && y > 193 && y < 245)
	{	

		music_pause();	//暂停播放
		change_state(STOP);
		
	}
}

void stop_process(unsigned x, unsigned y)	//在STOP状态时是”继续播放“而不是”播放“
{
	if(x > 780 && x <800 && y > 0 && y < 16)	//关闭页面
	{	
		music_stop();	//停止播放
		change_state(MAIN_CONTROL);
	}
	else if(x > 520 && x < 574 && y > 193 && y < 245)
	{	
		music_continue();	//继续播放
		change_state(PLAYING);
	}
}

void digital_albun_process(void)
{
	slide_show();	//在DIGITAL_ALBUM状态下按任意键播放幻灯片
	change_state(MAIN_CONTROL);//上述函数结束表明退出播放
}


void key_test_process(unsigned x, unsigned y)
{
	if(x > 90 && x <230 && y > 80 && y < 130)
		key_test(0);
	else if(x > 430 && x <570 && y > 80 && y < 130)
		key_test(1);
	else if(x > 90 && x <200 && y > 80 && y < 260)
		key_test(2);
	else if(x > 430 && x <570 && y > 80 && y < 260)
		key_test(3);
	else if(x > 700 && x <800 && y > 420 && y < 460)
		change_state(MAIN_CONTROL);	//返回
}