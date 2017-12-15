/**
*	Date:			2017/07/31
*	FileName:		snake_logic.c
*	Author: 		Leytou
*	Version:		1.0
*	Description: 	贪吃蛇控制逻辑
**/

#include "snake_logic.h"
#include "snake_ui.h"

Food food;					//食物位置信息
int head_x = 0;				//蛇头位置全局信息副本
int head_y = 0;				
int food_flag = 0;			//食物与身体是否重复标志
int boom_flag = 0;			//自身碰撞标志
int snake_len = INIT_LEN;	//蛇长
int direction = KEY_RIGHT;	//前进方向


static void dir_change();				/*读取键盘输入，并且改变方向*/
static int snake_go(Snode *head);		/*蛇前进一步（每时间周期调用）*/
static void food_refresh(Snode *head);	/*食物刷新（吃到食物后调用）*/
static void boom_cmp(Snode *node);		/*碰自己检测（遍历调用） */
static void food_cmp(Snode *node);		/*食物占位检测（遍历调用） */

/*游戏初始蛇身初始化*/
Snode *snake_init()
{
	/*蛇身信息结构体*/
	Sinfo s_info = {
		.x = INIT_X,					//链表头部的x,y与蛇的头部x,y保持相同
		.y = INIT_Y
	};
	
	Snode *SA = create_node(s_info);	//创建双向循环链表表头。

	int i = 1;
	s_info.x = INIT_X;
	for(; i <= INIT_LEN; ++i)			//根据初始蛇长，初始蛇身结点与坐标
	{
		s_info.y = INIT_Y - INIT_LEN + i;
		//用蛇身位置信息小结构体包装成大结构体结点，并插入到链表中
		insert_head(SA, create_node(s_info));
	}
	return SA;
}

/*游戏初始化*/ 
void game_init(Snode *head)
{
	ui_init();								//curses库初始设置
	fcntl(0,F_SETFL, O_NONBLOCK);			//getch()非阻塞
	srand((unsigned int)time(NULL));		//随机数种子设定
	food_refresh(head);						//食物刷新
}
 
/*游戏进行中，循环*/
void game_playing(Snode *head)												 
{
	int life = LIVE;		//生死标志
	
	do
	{
		dir_change();			//读取键盘输入，并且改变方向
		
		life = snake_go(head);	//前进一步

		frame_refresh(head);	//画面刷新
		
		usleep((int)(500.0/SPEED*1000.0));	//延时
		
	}while( LIVE == life );	//活着，循环
	
}

/*游戏结束（死亡时调用）*/
void game_over()
{	
	ui_over();				//显示游戏结束画面
	
	/*可在此处加入对蛇的内存进行释放*/
		
	exit(EXIT_SUCCESS);		//退出程序
}

/*读取键盘输入，并且改变方向*/
static void dir_change()
{
	int key = getch();		//读取按键，已设置非阻塞
	
	switch(key)				//对有效输入进行是否反向判断, 不合法则return而不改变方向
	{	
		case KEY_UP:	if(direction == KEY_DOWN)	return;		break;	//方向无效则return,否则有效break;		
		case KEY_DOWN:	if(direction == KEY_UP)		return;		break;
		case KEY_LEFT:	if(direction == KEY_RIGHT)	return;		break;
		case KEY_RIGHT:	if(direction == KEY_LEFT)	return;		break;
		case LOCAL_KEY_EXIT:	game_over();		break;		//按下exit退出游戏
		default:		return;
	}
	
	direction = key;		//	改变方向

}

/*蛇前进一步（每时间周期调用）*/
static int snake_go(Snode *head)
{
	switch(direction)
	{
		case KEY_UP:	head->s_info.x--; break;
		case KEY_DOWN:	head->s_info.x++; break;
		case KEY_LEFT:	head->s_info.y--; break;
		case KEY_RIGHT:	head->s_info.y++; break;
		default:	break;	
	}
	
	/*更新全局变量记录的头部坐标*/
	head_x = head->s_info.x;
	head_y = head->s_info.y;
	
	/*边界碰撞检测*/
	if( head->s_info.x <= 0 || head->s_info.x >= MAP_WIDTH || \
		head->s_info.y <= 0 || head->s_info.y >= MAP_LENGTH )
	{
		return DIED;
	}
	
	/*吃食物检测*/
	else if( head->s_info.x == food.x && head->s_info.y == food.y )
	{
		++snake_len;
		food_refresh(head);
	}
	else
	{
		delete_tail(head);							//删尾
	}
	
	insert_head(head, create_node(head->s_info));	//增头
	
	/*自身碰撞检测，用蛇头坐标和蛇头到蛇尾逐个比较*/
	/*遍历后boom_flag至少为1，因为头与头当然重合，若碰撞蛇身则值为2（即>1），死亡*/
	/*自身碰撞检测需要在删尾后，否则蛇身闭合（咬到蛇尾）也会误判GAME OVER。*/
	boom_flag = 0;	
	traverse(head, boom_cmp);	
	if(boom_flag > 1)								
		return DIED;
	
	return LIVE;
}

/*食物刷新（吃到食物后调用）*/
static void food_refresh(Snode *head)
{
	do{
		food_flag = 0;
		food.x = 1+rand()%(MAP_WIDTH-1);	//食物生成范围([1，MAP_WIDTH-1],[1，MAP_LENGTH-1])
		food.y = 1+rand()%(MAP_LENGTH-1);
		traverse(head, food_cmp);			//食物占位检测
	}
	while( 1 == food_flag );				//食物位置没有碰到身体则结束循环
}

/* 碰自己检测（遍历调用） */
static void boom_cmp(Snode *node)
{
	if(node->s_info.x == head_x && node->s_info.y == head_y)
		boom_flag++;
}

/* 食物占位检测（遍历调用） */
static void food_cmp(Snode *node)
{
	if(node->s_info.x == food.x && node->s_info.y == food.y)
		food_flag = 1;

}

