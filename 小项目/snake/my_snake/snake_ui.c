#include "my_snake.h"
//#include "snake_ui.h"

void ui_game_over();
//产生食物
void product_rand_food(void);

//初始化屏幕，生成一个游戏的界面
bool init_game()
{
	//curse的初始化
	initscr();
	noecho();
	//cbreak();
	curs_set(0);	//隐藏光标
	timeout(50);//非阻塞模式
	//crmode();		//立即读取模式
	keypad(stdscr, TRUE);
	refresh();
	//打印游戏画面
	ui_game();
	
}

//不打印链表头
void print_node(struct list_head *head)
{
	Snake *p;
	struct list_head *pos;
	
	char body = '0';
	list_for_each(pos, head)
	{
		p = list_entry(pos, Snake, list);
		/*if(p->snake_info.x == head_x && p->snake_info.y == head_y)
		{
			body = '*';
			mvprintw(p->snake_info.y, p->snake_info.x,"%c", body);
		}*/
		
			body = '0';
			mvprintw(p->snake_info.y, p->snake_info.x,"%c", body);
		
		//printf("x = %d,y = %d\n", p->snake_info.x, p->snake_info.y);
	}
	refresh();
	
}
//打印一个游戏画面
void ui_game()
{
	int i, j;
	//打印一个界面
	for(i =0; i<=COLS_MAX; i++)
	{
		mvaddch(0, i, '+');
		mvaddch(LINE_MAX, i, '+');
		
	}
	for(i =0; i<=LINE_MAX; i++)
	{
		
		mvaddch(i, 0, '+');
		mvaddch(i, COLS_MAX, '+');
	}
	//getch();
	refresh();
}

//刷新游戏画面
void refresh_game(Snake *head)
{
	//清屏
	clear();
	//重新打印画面
	ui_game();
	print_node(&head->list);
	if(flag_food ==1)
	{
		flag_food =0;
		product_rand_food();
		mvaddch(food_y, food_x, '&');
	}
	else
		mvaddch(food_y, food_x, '&');
	refresh();
}

//游戏结束
void game_over()
{
	//ui_game_over();
	ui_game_over();
	timeout(-1);  //设置为阻塞模式
	getch();
	endwin();
}

//游戏退出画面
void ui_game_over()
{
	mvprintw(7, 25,"fail");
}

//食物
void product_rand_food(void)
{
	srand(time(0));
	srand_food.x = rand()%50;
	srand_food.y = rand()%15;
	food_x = srand_food.x;
	food_y = srand_food.y;
	//mvaddch(s, srand_food.x, '&');
	return;
}




















