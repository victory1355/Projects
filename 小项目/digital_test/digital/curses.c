#include "search_file.h"
#include "search_file.c"
#include "curses.h"

//两个表头全局变量
File_info *head_photo;
File_info *head_music;

//打印界面
void print()
{
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
	mvaddstr(19, 30, "q or Q to exit");
	
}
//终端的初始化
void init_term()
{
	initscr();
	noecho();
	crmode();
	cbreak();
	keypad(stdscr, true);
	print();
	
	timeout(-1);
	refresh();
	
}
//打印图片文件的内容
void print_node_content_photo(File_info *head)
{
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


void key_control()
{
	char name_music[200];
	char name_photo[200];
	char path_photo[200];
	char path_music[200];
	int flag = 0;
	char key;  
	do
	{
		key = getch();//阻塞模式
		switch(key)
		{
					//打开图片
			case '1':
					echo();
					mvaddstr(14, 4, "input photo name:");
					while(1)
					{
						scanw("%s", name_photo);
						break;
					}
					 int reto = open_photo(head_photo, name_photo);
					 if(reto == -1)
					 {
						 //提示打开图片失败
						 mvaddstr(22, 4, "sorry ! open photo fail");
					 }
					 noecho();
					 break;
					 //删除图片
			case '2':
					echo();
					mvaddstr(14, 4, "input photo name:");
					refresh();
					//while(1);
					bzero(name_photo, sizeof(name_photo));					
					//while(getch()!= '\n');
					scanw("%s", name_photo);					
					//mvprintw(22, 35, "%s", name_photo);
					refresh();
					
					 int retd = delete_photo(head_photo, name_photo);					 
					 if(retd == -1)
					 {
						 //提示删除图片失败
						 mvaddstr(22, 4, "sorry ! delete photo fail");
					 }
					 noecho();
					 break;
					 //复制图片
			case '3':
					echo();
					mvaddstr(14, 4, "input photo name and dest path:");
					while(1)
					{
						scanw("%s%s", name_photo, path_photo);
						break;
					}
					int retc = copy_photo(head_photo, name_music, path_photo);
			        if(retc == -1)
					{
						//提示复制图片失败
						 mvaddstr(22, 4, "sorry ! copy photo fail");
					}
					noecho();
					break;
					//播放音乐失败
			case '4':
					echo();
					mvaddstr(14, 4, "input music name:");
					while(1)
					{
						scanw("%s", name_music);
						break;
					}
					 int reto1 = play_music(head_music, name_music);
					 if(reto1 == -1)
					 {
						 //提示播放失败
						 mvaddstr(22, 4, "sorry ! play music fail");
					 }
					 noecho();
					 break;
					 //删除音乐
			case '5':	
					echo();
					mvaddstr(14, 4, "input music name:");
					while(1)
					{
						scanw("%s", name_music);
						break;
					}
					 int reto2 = delete_music(head_music, name_music);
					 if(reto2 == -1)
					 {
						 //提示失败
						 mvaddstr(22, 4, "sorry ! delete music fail");
					 }
					 noecho();
					 break;
					 //复制音乐文件
			case '6':
					echo();
					mvaddstr(14, 4, "input music name and dest path:");
					while(1)
					{
						scanw("%s%s", name_music, path_music);
						break;
					}
					 int reto3 = copy_music(head_music, name_music, path_music);
					 if(reto3 == -1)
					 {
						 //提示失败
						 mvaddstr(22, 4, "sorry ! copy music fail");
					 }
					 noecho();
					 break;
			case 'q':
			case 'Q':flag = 1;break;
			default:break;
		}
		if(flag == 1)
			break;
		
	}while(1);
	endwin();
}




int main(int argc, char *argv[])
{
	//测试代码
	File_info *head_photo = create_file_head();
	if(head_photo == NULL)
	{
		perror("create fail");
		return -1;
	}
	File_info *head_music = create_file_head();
	if(head_music == NULL)
	{
		perror("create fail");
		return -1;
	}
	
	tree_find_file(argv[1], head_photo, head_music);
	
	init_term();
	print_node_content_music(head_music);
	print_node_content_photo(head_photo);
	
	key_control();
	return ;
}












