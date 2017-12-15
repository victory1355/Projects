#include "digital.h"

//终端的初始化
void init_term()
{
	initscr();
	noecho();
	crmode();
	cbreak();
	keypad(stdscr, true);
	//print();
	curs_set(0);
	timeout(-1);
	refresh();
	
}
//打印图片文件的内容
void print_node_content_photo(File_info *head)
{
	if(head == NULL)
		return;
	struct list_head *pos=NULL;
	int line = 4;
	int cols = 4;
	list_for_each(pos, &head->list)
	{	
		File_info *file = list_entry(pos, File_info, list);
		mvprintw(line, cols, "      %-20s%d",file->file_group.file_name, file->file_group.file_size);
		line++;
		if(line == 19)
		{
			line = 4;
			cols = 20;
		}
	}	
}
//打印音乐文件的内容
void print_node_content_music(File_info *head)
{
	if(head == NULL)
		return ;
	struct list_head *pos=NULL;
	int line = 4;
	int cols = 35;
	list_for_each(pos, &head->list)
	{	
		File_info *file = list_entry(pos, File_info, list);
		mvprintw(line, cols, "      %-20s%d",file->file_group.file_name, file->file_group.file_size);
		line++;
		if(line == 19)
		{
			line = 4;
			cols = 20;
		}	
	}	
}

//自动模式界面
void auto_ui()
{
	mvprintw(LINES/2, COLS/2-15, "press anykey to do what you want");
	mvprintw(LINES/2+1, COLS/2-15, "       'c':continue");
	mvprintw(LINES/2+2, COLS/2-15, "       's':stop");
	mvprintw(LINES/2+3, COLS/2-15, "       'e':exit");
	refresh();
}



//打印界面
void print()
{
	clear();
	int i;
	//界面的轮廓
	mvaddstr(1,25,"welcome to digital photo");
	//左边是音乐
	mvaddstr(3,4, "photo:name               size");
	//右边是图片
	mvaddstr(3,35, "music:name               size");
	for(i=2; i<=75; i++)
	{
		mvaddch(2, i, '+');
		mvaddch(20, i, '+');
	}
	for(i=2; i<=20; i++)
	{
		mvaddch(i, 2, '+');
		mvaddch(i, 75, '+');
	}
	//操作
	mvaddstr(16, 30, "file operation");
	for(i=2; i<75; i++)
	{
		mvaddch(15, i, '+');
	}
	mvaddstr(17, 15, "1,show photo    2,delete photo    3,copy photo");
	mvaddstr(18, 15, "4,play music    5,delete music    6,copy music");
	mvaddstr(19, 15, "'q' or 'Q' to exit              'a':auto_model");
	
}






