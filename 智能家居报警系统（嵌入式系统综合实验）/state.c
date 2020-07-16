#include "state.h"

void show(unsigned cur)
{
	switch(cur)		//调用显示图像的函数
	{
		case MAIN_FACE :
			lcd_show_bmp("main.bmp");
			break;

		case MAIN_CONTROL :
			lcd_show_bmp("main_control.bmp");
			break;

		case LIGHT_CONTROL:
			lcd_show_bmp("light_control.bmp");
			break;
		case VIDEO_CENTOR :
			lcd_show_bmp("video_centor.bmp");
			break;
		case ALTER_CONTROL :
			lcd_show_bmp("alter_control.bmp");
			break;
		case DOOR_WINDOW :
			lcd_show_bmp("door_window.bmp");
			break;
		case INFOR :
			lcd_show_bmp("door_window_infor.bmp");
			break;
		case BUZZER :
			lcd_show_bmp("door_window_buzzer.bmp");
			break;
		//case INFOR_BEEP :
		//	lcd_show_bmp("door_window_buzzerandinfor.bmp");
		//	break;

		case BGM :
			lcd_show_bmp("BGM_set.bmp");
			break;
		case PLAY_MUSIC :
			lcd_show_bmp("play_music_stop.bmp");	//进入播放音乐时显式暂停播放的界面
			break;
		case PLAYING :
			lcd_show_bmp("play_music_playing.bmp");
			break;
		case STOP :
			lcd_show_bmp("play_music_stop.bmp");
			break;

		case DIGITAL_ALBUM :
			lcd_show_bmp("image0.bmp");
			break;

		case KEY_TEST :
			lcd_show_bmp("test_key_control.bmp");
			break;
			
		default :;

	}
	return;
}

void change_state(unsigned int sta)
{
	state = sta;
	return;
}
