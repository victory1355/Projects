#include "photo_ui.h"


//UI界面的具体实现


//内容显示的界面
void content_ui()
{
	int i, j;
	mvaddstr(0, 5,"welcome to my digital photo");
	for(i=0; i<50;i++)
	{
		mvaddch(0,i, '+');
		mvaddch(15,i, '+');
	}
	for(i=0; i<15;i++)
	{
		mvaddch(i,0, '+');
		mvaddch(i,50, '+');
	}
	
	return;
}




