#ifndef __DIGITAL_H
#define __DIGITAL_H

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include <curses.h>
#include "list.h"

//#include <stdio.h>
//#include <unistd.h>
//#include <dirent.h>
#include <errno.h>
#include <signal.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <string.h>
#include <strings.h>
#include <fcntl.h>

#include <sys/wait.h>
//#include <stdlib.h>

//图片，音乐链表的数据结构
struct file
{
	char path[100];  //文件所在的路径
	char file_name[50];
	unsigned short file_size;
};
typedef struct file_info
{
	struct file file_group;
	struct list_head list;      //内核链表，记住不要定义为指针类型
}File_info;

//路径的最大长度
#define MAX_PATH   200

//两个表头全局变量
File_info *head_photo;
File_info *head_music;

/*-------------------------------------------------------------------------*/

//自动模式的控制变量
int flag_stop;
int flag_continue;
int flag_exit;
int flag_work;

//USB设备检测标志
int flag_usb;

//int flag;

//随机播放音乐
int  rand_music(File_info *music_head2) ;
//浏览图片
int  brower_photo(File_info *photo_head2) ;
//进入自动模式
int auto_model(File_info *head_music1, File_info *head_photo1);

//信号处理函数
void stop_music_photo(int sig);
void continue_music_photo(int sig);
void exit_auto(int sig);

//发送信号
int send_signal();
//自动模式界面
void auto_ui();

//挂载U盘
void mount_usb();
//卸载U盘
void umount_usb();
//U盘检测
void found_usb(int sig);

/*-------------------------------------------------------------------------*/



//通过名字查找对应的文件, 返回文件的绝对路径
int  search_by_name(File_info *node, char *name, char *return_path) ;


//提取MP3和JPG图片,并建立两条链表
void tree_find_file(char *dir ,File_info *head_photo, File_info *head_music);
//创建表头
File_info * create_file_head(void);
//创建图片信息链表
int creat_file_list_photo(File_info *head, char *name, int size, char *path);
//创建音乐信息链表
int creat_file_list_music(File_info *head, char *name, int size, char *path);

//删除节点
int  delete_node(File_info *head, char *name);
//遍历链表,使用回调函数，多次使用
void travel_list(File_info *head);
//打印文件信息
void print_file_info(File_info *node);


//删除音乐，输入音乐文件名
int  delete_music(File_info *head, char *file_name);
//复制音乐到指定的目录,文件名+绝对路径，成功返回0，失败返回-1
int  copy_music(File_info *head, char *name, char *dest_path);
//播放音乐,输入音乐名，成功返回0，失败返回1
int play_music(File_info *head ,char *file_name);



//复制图片到指定的目录,成功返回0，失败返回-1
int  copy_photo(File_info *head, char *name, char *dest_path);
//删除图片
int  delete_photo(File_info *head, char *file_name);
//打开图片
int open_photo(File_info *head , char *file_name);

//文件复制的接口,成功返回0，失败返回-1
int copy(char *src, char *dest);


//打印界面
void print();
//终端的初始化
void init_term();
//打印图片的内容
void print_node_content_photo(File_info *head);
//打印音乐文件的内容
void print_node_content_music(File_info *head);
//按键控制
void key_control(File_info *head_music1, File_info *head_photo1);


#endif