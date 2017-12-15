/**
*	Date:			2017/07/31
*	FileName:		snake_ui.h
*	Author: 		Leytou
*	Version:		1.0
*	Description: 	贪吃蛇画面绘制
**/


#ifndef __SNAKE_UI_H
#define __SNAKE_UI_H

#include <curses.h>			//curses图形库
#include "snake_logic.h"
#include "snake_link.h"

/*地图左上角坐标*/
#define MAP_X	3
#define MAP_Y	10

/*地图长宽*/
#define MAP_LENGTH	40		//Y
#define MAP_WIDTH	28		//X

/*蛇的起始坐标*/
#define INIT_X		(MAP_WIDTH/2)
#define INIT_Y		(MAP_LENGTH/2)

/*蛇长度显示坐标*/
#define LEN_X		(-1)
#define LEN_Y		(MAP_LENGTH/2 - 6)

extern unsigned int rudong_i;

/*初始化UI*/
void ui_init();
/*画面刷新*/
void frame_refresh(Snode *head);
/*游戏结束UI*/
void ui_over();

#endif