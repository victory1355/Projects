#include "my_snake.h"
#include "snake_ui.h"
//#include "snakelist.h"

//蛇的身体检测
int body_touch(Snake *head);

//头插法插入节点
bool insert_node(struct list_head *head, struct list_head *_new)
{
	list_add(_new, head);
}
//删除节点
bool delete_node(struct list_head *del)
{
	list_del(del);
}

//创建一条蛇,不包括表头
bool create_snake(struct list_head *head, int snake_len)
{
	int i;
	Snake *p;
	for(i=1; i<=snake_len; i++)
	{
		p = (Snake *)malloc(sizeof(Snake));
		
		p->snake_info.y = INIT_Y;
		p->snake_info.x = INIT_X+i;
		INIT_LIST_HEAD(&p->list);
		
		insert_node(head, &p->list);
	}
	//print_node(head->next);
	//getch();
	return true;
}


/*读取键盘输入，并且改变方向*/
int  dir_change()
{
	int key = getch();		//读取按键，已设置非阻塞
	
	switch(key)				//对有效输入进行是否反向判断, 不合法则return而不改变方向
	{	
		case KEY_UP:	if(direction_ == KEY_DOWN)	return 0;	break;	//方向无效则return,否则有效break;		
		case KEY_DOWN:	if(direction_ == KEY_UP)	return 0;	break;
		case KEY_LEFT:	if(direction_ == KEY_RIGHT)	return 0;	break;
		case KEY_RIGHT:	if(direction_ == KEY_LEFT)	return 0;	break;
		case 'q':return 27;		
		case 'Q':return 27;		
		default:return 0;
	}
	
	direction_ = key;		//	改变方向

}
//蛇的移动
bool snake_move(Snake *head)
{
	
	switch(direction_)
	{
		case KEY_UP:head->snake_info.y--;break;
		case KEY_DOWN:head->snake_info.y++;break;
		case KEY_LEFT:head->snake_info.x--;break;
		case KEY_RIGHT: head->snake_info.x++;break;
		case 27: flag_exit_ = 1;return true;
		default:break;
		
	}
	head_x = head->snake_info.x;
	head_y = head->snake_info.y;
	//如果进入非法区域则退出
	if(head->snake_info.x >= COLS_MAX+1 || head->snake_info.x <= COLS_MIN-1)
	{
		flag_exit_ = 1;
		return false;
	}else if(head->snake_info.y >= LINE_MAX+1 || head->snake_info.y <= LINE_MIN-1)
	{
		flag_exit_ = 1;
		return false;
	}else if(head->snake_info.x == srand_food.x && head->snake_info.y == srand_food.y){
		flag_food=1;
	}
	//如果触碰到自己则退出
	int flag = body_touch(head);
	if(flag == 1)
	{
		flag_exit_ =1;
		return false;
	}
	
	if(flag_exit_ == 0 && flag_food != 1)
	{
		//删除尾节点
		struct list_head *del = head->list.prev;
		delete_node(del);
	}		
	
	Snake *p = (Snake *)malloc(sizeof (Snake));
	p->snake_info.x = head->snake_info.x;
	p->snake_info.y = head->snake_info.y;
	//头插法插入节点
	insert_node(&head->list, &p->list);
	
	usleep(50000);   //每50毫秒刷新一次画面
	return true;
}


//蛇的身体检测
int body_touch(Snake *head)
{
	
	Snake *p;
	struct list_head *pos;
	
	list_for_each(pos, head->list.next)
	{
		p = list_entry(pos, Snake, list);
		if(head_x == p->snake_info.x && head_y == p->snake_info.y)
			return 1;
	}
	return 0;
}













































