#include "state.h"

void show(unsigned cur)
{
	switch(cur)
	{
		case MAIN_FACE :
			lcd_show_bmp("main.bmp");
			break;
		case CONTROL :
			lcd_show_bmp("control.bmp");
			break;
		case DOOR_WINDOW :
			lcd_show_bmp("door_window.bmp");
			break;
		case INFOR :
			lcd_show_bmp("door_window_infor.bmp");
			break;
		case BEEP :
			lcd_show_bmp("door_window_beep.bmp");
		case INFOR_BEEP :
			lcd_show_bmp("door_window_beepandinfor.bmp");
		default :;

	}
	return;
}

void change_state(unsigned int sta)
{
	state = sta;
	return;
}
