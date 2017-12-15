/**
*	Date:			2017/07/31
*	FileName:		snake_ui.c
*	Author: 		Leytou
*	Version:		1.0
*	Description: 	贪吃蛇画面绘制
**/

#include "snake_ui.h"

/*蠕动效果计数器*/
unsigned int rudong_i = 0;

/*打印蛇*/ 
static void snake_print(Snode *node);
/*边界打印*/
static void ui_border();

/*初始化UI*/
void ui_init()
{
	initscr();								//curses初始化
	curs_set(0);							//隐藏光标
	clear();								//清屏
	refresh();								//屏幕刷新
	
	crmode();								//立即读取模式
	keypad(stdscr, TRUE);					//可以使用键盘上的一些特殊字元
    noecho();								//关闭回显
}

/*画面刷新*/
void frame_refresh(Snode *head)
{
	clear();													//清屏
	
	ui_border();												//打印边框
	
	rudong_i = 0;
	traverse(head, snake_print);								//打印蛇
	
	mvprintw(MAP_X+food.x , (MAP_Y+food.y)*2 , "*");				//打印食物
	
	mvprintw(MAP_X+LEN_X , (MAP_Y+LEN_Y)*2 , "Length:%5d", snake_len);	//显示蛇长度 
	
	refresh();	
}   
 
/*游戏结束UI*/
void ui_over()
{
	clear();
	mvprintw(MAP_X+MAP_WIDTH/2, (MAP_Y+MAP_LENGTH/2)*2, "GAME OVER");
	refresh();
	sleep(1);
	endwin();				//结束curses
}
  
/*打印蛇*/ 
static void snake_print(Snode *node)
{

	char snake_ch = (rudong_i++ % 2 == 0) ? 'O' : 'o';			//蠕动效果
	
	if( node->s_info.x == head_x && node->s_info.y == head_y)	//头部用@
		snake_ch = '@';
	
	mvprintw(MAP_X+node->s_info.x, (MAP_Y+node->s_info.y)*2, "%c", snake_ch);
}

/*边界打印*/
static void ui_border()
{
	int row = 0;
	int col = 0;
	for(row=0, col=0; 	row <= MAP_WIDTH; ++row)				mvprintw( MAP_X+row, (MAP_Y+col)*2, "|");	//左边界
	for(row=0, col=MAP_LENGTH;	row <= MAP_WIDTH; ++row)		mvprintw( MAP_X+row, (MAP_Y+col)*2, "|");	//右边界
	for(row=0, col=0;	col <= MAP_LENGTH; ++col)				mvprintw( MAP_X+row, (MAP_Y+col)*2, "-");	//上边界
	for(row=MAP_WIDTH, col=0;	col <= MAP_LENGTH; ++col)		mvprintw( MAP_X+row, (MAP_Y+col)*2, "-");	//下边界
}

