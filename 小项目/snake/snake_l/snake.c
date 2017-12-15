/**
*	Date:			2017/07/31
*	FileName:		snake.c
*	Author: 		Leytou
*	Version:		1.0
*	Description: 	贪吃蛇主函数
**/
#include "snake_logic.h"

int main()
{
	/*创建一条蛇*/
	Snode *SA = snake_init();					
	
	game_init(SA);
	
	game_playing(SA);
	
	game_over();
	
	return 0;
}
