#ifndef __MY_SNAKE
#define __MY_SNAKE

#include "list.h"
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "snake_ui.h"
#include <time.h>
#include <stdlib.h>


//链表的头节点的坐标
int head_x;
int head_y;

//蛇的坐标，根据用户的输入不断的更新
struct snake_node
{
	int x;
	int y;
};
//链表的节点信息
typedef struct snake
{
	struct list_head list;
	struct snake_node snake_info;
}Snake;

//食物的数据结构
typedef struct snake_food
{
	int x;
	int y;
}Food;

//食物
Food srand_food;

//食物标志
int flag_food;
//具体功能函数的实现
int flag_exit_;
//链表的头节点
//Snake *HEAD_;
//按键的信息
int direction_;

//纪录最新的坐标值
int head_x;
int head_y;
/*读取键盘输入，并且改变方向*/
int dir_change();


static int food_x;
static int food_y;

//刷新游戏画面
void refresh_game(Snake *head);

//创建一条蛇
bool create_snake(struct list_head * head, int snake_len);
//遍历链表，采用内联函数
void treavers(Snake* (func)(struct list_head *head));
//在终端机上打印所有的节点
void print_node(struct list_head *head);
//按键的检测
void key_scaner();
//蛇的移动
bool snake_move(Snake* head);
//定时函数
void set_time();
//创建节点
Snake* create_node(Snake* node);
//插入节点
bool insert_node(struct list_head *head, struct list_head *_new);
//删除节点
bool delete_node(struct list_head *del);

//初始化屏幕，生成一个游戏的界面
bool init_game();
//游戏结束
void game_over();

#endif