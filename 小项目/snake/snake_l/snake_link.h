/**
*	Date:			2017/07/31
*	FileName:		snake_link.h
*	Author: 		Leytou
*	Version:		1.0
*	Description: 	调用内核链表的贪吃蛇链表的封装
**/

#ifndef __SNAKE_LINK_H
#define __SNAKE_LINK_H

#include <stdio.h>
#include <stdlib.h>

#include "list.h"	//内核链表

/*蛇结点坐标小结构体*/
typedef struct snode_info
{
	int x;		
	int y;
}Sinfo;


/*蛇身结点大结构体*/
typedef struct snode
{
	Sinfo s_info;			//蛇结点信息
	struct list_head list;	//内核链表结点
	
}Snode;

/*创建一个大结点*/
Snode *create_node(Sinfo s_info);

/*头插法，插入一个大结点*/
int insert_head(Snode *head, Snode *new);

/*遍历打印*/
int traverse(Snode *head, void (*func)(Snode *));

/*删除尾结点*/
int delete_tail(Snode *head);

#endif