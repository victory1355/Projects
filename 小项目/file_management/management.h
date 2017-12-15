#ifndef __MANAGEMENT_H
#define __MANAGEMENT_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


//文件信息链表结点设计
typedef struct file_node
{
	struct list_head list;
	//文件信息
	char name[30];  //文件名
	char type[20];  //文件类型
	char  size[10];      //文件大小
}File_node;
//路径链表结点设计
typedef struct path_node
{
	struct list_head list;
	
	//路径信息
	char path[100];   //目录名
}Path_node;




//按键
 int num;
 //切换的目录名
 char doc[200];
 
 
void create_head(struct list_head *path_head, struct list_head *filename_head);
 
//主界面
void display(struct list_head *path_head);

//进入子目录
void next(struct list_head *path_head, struct list_head *filename_head);
//返回上一级目录
void back(struct list_head *path_head, struct list_head *filename_head);
//查看文件信息
void show_file(struct list_head *filename);
//修改文件名
void change_name();

//退出
void quit(struct list_head *path_head, struct list_head *filename_head);

//更新文件信息
void update_file_info(char *p);

//获取文件信息，并创建文件信息链表
int get_file_info(struct list_head *filename_head);


//遍历并打印结点信息
void show_file_node(struct list_head *filename_head);

//创建路径链表
void get_path(struct list_head *path_head, struct list_head *filename_head);

//获取当前路径
int get_first_path(struct list_head * path_head);

//打印当前目录下的所有子目录
int show_doc(struct list_head *path_head, struct list_head *filename_head);

//遍历并打印路径信息
void show_path_node(struct list_head *path_head);


//销毁链表，但是保留头结点
int  destroy(struct list_head *path_head);






#endif