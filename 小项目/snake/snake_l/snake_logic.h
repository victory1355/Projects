/**
*	Date:			2017/07/31
*	FileName:		snake_logic.h
*	Author: 		Leytou
*	Version:		1.0
*	Description: 	贪吃蛇控制逻辑
**/

#ifndef __SNAKE_LOGIC_H
#define __SNAKE_LOGIC_H

#include <string.h>
#include <unistd.h>
#include <fcntl.h>			//getch()不阻塞包含该库
#include <time.h> 
#include "snake_link.h"

#define INIT_LEN	10


#define LOCAL_KEY_EXIT	27										//ESC键键值

#define SPEED 5			//速度	值越大越快

#define DIED	0			//蛇死亡
#define LIVE	1			//蛇活着

typedef struct food
{
	int x;
	int y;
}Food;

extern Food food;
extern int head_x;
extern int head_y;
extern unsigned int rudong_i;
extern int snake_len;

/*游戏初始蛇身初始化*/
Snode *snake_init();
/*游戏初始化*/
void game_init();
/*游戏进行中，循环*/
void game_playing(Snode *head);	
/*游戏结束（死亡时调用）*/
void game_over();
/* 碰自己检测（遍历调用） */
//void boom_cmp(Snode *node);
/* 食物占位检测（遍历调用） */
//void food_cmp(Snode *node);


#endif