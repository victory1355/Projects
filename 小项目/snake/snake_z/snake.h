#ifndef __SNAKE__
#define __SNAKE__

#include<stdio.h>
#include<stdlib.h>
#include"kernel_list.h"
#include<curses.h>
#include<stdbool.h>
#include<time.h>
#include<signal.h>
#include <sys/time.h> 



//内核链表

//__list_add(struct list_head *new,
//				struct list_head *prev,
//				struct list_head *next)
//把两个结点连起来，传入前后结点和新结点
	

//把结点删除	
//static inline void list_del(struct list_head *entry)
//{
//	__list_del(entry->prev, entry->next);
//	entry->next = (void *) 0;
//	entry->prev = (void *) 0;
//}				
		

//把结点占时断开		
//static inline void list_del_init(struct list_head *entry)
//{
//	__list_del(entry->prev, entry->next);
//	INIT_LIST_HEAD(entry);
//}	


//移动某个结点
//static inline void list_move(struct list_head *list,
//				struct list_head *head)
//{
//	__list_del(list->prev, list->next);
//	list_add(list, head);
//}		移动结点

//判断是否为空，是返回1，否返回0
//static inline int list_empty(struct list_head *head)
//{
//	return head->next == head;
//}		
				
//判断位置
typedef struct pos
{
	struct list_head list;
	int x;
	int y;
	
}Pos;


void create_screen();

void create_snake();

//void create_node(int x,int y);

void snake_move();

void show_snake();

void speed_set();  

void insert_tail();

int search();

void food();

void control();

//void count_down();
#endif
