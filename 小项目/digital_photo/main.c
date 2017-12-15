#include <stdio.h>
#include <curses.h>
//#include "photo_ui.h"



//内容显示的界面
void content_ui()
{
	int i;
	mvaddstr(1, 23,"welcome to my digital photo");
	mvaddstr(4, 20,"content");
	
	for(i=3; i<=70;i++)
	{
		mvaddch(3,i, '+');
		mvaddch(15,i, '+');
	}
	for(i=3; i<=20;i++)
	{
		mvaddch(i,3, '+');
		mvaddch(i,70, '+');
	}
	
	return;
}

//图片显示的界面
void photo_ui()
{
	int i;
	mvaddstr(4, 55,"show photo");
	for(i=3; i<=15;i++)
	{
		mvaddch(i,50, '+');
	}
	
	return;
}
//文件操作的界面
void operation_ui()
{
	int i;
	mvaddstr(16, 30,"file operation");
	for(i=3; i<=70;i++)
	{
		mvaddch(20,i, '+');
	}

	return;
}

int main()
{
	initscr();
	noecho();
	timeout(-1);
	content_ui();
	photo_ui();
	operation_ui();
	refresh();
	getch();
	endwin();
	
	return 0;
}












