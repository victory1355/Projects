#include "my_snake.h"
//#include "snake_ui.h"


//具体功能函数的实现

int flag_exit_ = 0;
//一开始移动的方向
int	direction_ = KEY_RIGHT;
//按键控制
int flag_exit_dir =0;

//食物
Food srand_food;
//食物标志
int flag_food = 0;

static int food_x;
static int food_y;


int main()
{
	
	
	//链表的头节点
	Snake *head = (Snake *)malloc(sizeof(Snake));
	if(head == NULL)
		return false;
	INIT_LIST_HEAD(&head->list);
	
	head_x = INIT_X;
	head_y = INIT_Y;
	
	head->snake_info.x = INIT_X;
	head->snake_info.y = INIT_Y;
	
	//生成一条蛇
	create_snake(&head->list, 6);
	
	//初始化游戏
	init_game();
	//产生食物
	product_rand_food();
	print_node(&head->list);
	//进入游戏
	while(1)
	{
		flag_exit_dir = dir_change();
		if(flag_exit_== 1  || flag_exit_dir == 27)
		{
			game_over();
			return 0;
		}	
		snake_move(head);
		//if(flag_food ==1)
		refresh_game(head);
			
	}
	return 0;
}



