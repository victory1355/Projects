#ifndef __CURSES
#define __CURSES


#include <curses.h>




//打印界面
void print();
//终端的初始化
void init_term();
//打印图片的内容
void print_node_content_photo(File_info *head);
//打印音乐文件的内容
void print_node_content_music(File_info *head);
//按键控制
void key_control();


#endif