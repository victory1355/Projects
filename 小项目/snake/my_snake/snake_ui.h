#ifndef __SNAKE_UI
#define __SNAKE_UI


bool init_game();
void print_node(struct list_head *head);
void ui_game();
void game_over();

//初始坐标
#define INIT_X   3
#define INIT_Y   3

//非法区域
#define LINE_MAX   15   //行
#define LINE_MIN   0
#define COLS_MAX   50    //列
#define COLS_MIN   0

#endif