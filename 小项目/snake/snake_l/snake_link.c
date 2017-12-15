/**
*	Date:			2017/07/31
*	FileName:		snake_link.c
*	Author: 		Leytou
*	Version:		1.0
*	Description: 	调用内核链表的贪吃蛇链表的封装
**/

#include "snake_link.h"

/*创建一个大结点*/
Snode *create_node(Sinfo s_info)
{
	Snode *node = (Snode *)malloc( sizeof(Snode) );
	if(NULL == node)
		return NULL;
	
	node->s_info.x = s_info.x;
	node->s_info.y = s_info.y;
	
	INIT_LIST_HEAD(&node->list);
	
	return node;
}

/*头插法，插入一个大结点*/
int insert_head(Snode *head, Snode *new)
{
	list_add(&new->list, &head->list);
	
	return 0;
}

/*遍历，大结构体头结点，与一个遍历时每次的操作函数*/
int traverse(Snode *head, void (*func)(Snode *))
{
	struct list_head *pos;
	
	list_for_each(pos, &head->list)
	{  
		Snode *tmp = list_entry(pos, Snode, list);  
		func(tmp);		//将大结构体结点转到func指向的函数执行对应操作
	}  
	return 0;
}

/*删除尾结点*/
int delete_tail(Snode *head)
{
	list_del(head->list.prev);
	return 0;
}

