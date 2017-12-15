#include "digital.h"

void key_control(File_info *head_music1, File_info *head_photo1)
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
					mvaddstr(14, 4, "                                                     ");
					mvaddstr(14, 4, "input photo name:");
					refresh();
					while(1)
					{
						scanw("%s", name_photo);
						break;
					}
					 int reto = open_photo(head_photo1, name_photo);
					 if(reto == -1)
					 {
						 //提示打开图片失败
						 mvaddstr(22, 4, "                                           ");
						 mvaddstr(22, 4, "sorry ! open photo fail");
						 refresh();
					 }
					 else 
					 {
						 //sleep(10);
						 print();
						 print_node_content_photo(head_photo1);
						 print_node_content_music(head_music1);
						 mvaddstr(22, 4, "                                           ");
						 mvaddstr(22, 4, " open photo successful");
						 refresh();
					 }
					 noecho();
					 break;
					 //删除图片
			case '2':
					echo();
					mvaddstr(14, 4, "                                                   ");
					mvaddstr(14, 4, "input photo name:");
					refresh();
					//while(1);
					bzero(name_photo, sizeof(name_photo));					
					//while(getch()!= '\n');
					scanw("%s", name_photo);					
					//mvprintw(22, 35, "%s", name_photo);
					refresh();
					 
					 int retd = delete_photo(head_photo1, name_photo);					 
					 if(retd == -1)
					 {
						 //提示删除图片失败
						 mvaddstr(22, 4, "                                           ");
						 mvaddstr(22, 4, "sorry ! delete photo fail");
						 refresh();
					 }
					 else
					 {
						 print();
						 //删除结点
						 delete_node(head_photo1, name_photo);
						 print_node_content_photo(head_photo1);
						 print_node_content_music(head_music1);
						 
						 mvaddstr(22, 4, "                                           ");
						 mvaddstr(22, 4, " delete photo successful");	
						 refresh();
					 }
						 
					 noecho();
					 break;
					 //复制图片
			case '3':
					echo();
					mvaddstr(14, 4, "                                                         ");
					mvaddstr(14, 4, "input photo name and dest path:");
					refresh();
					while(1)
					{
						scanw("%s%s", name_photo, path_photo);
						break;
					}
					int retc = copy_photo(head_photo1, name_photo, path_photo);
			        if(retc == -1)
					{
						//提示复制图片失败
						mvaddstr(22, 4, "                                           ");
						mvaddstr(22, 4, "sorry ! copy photo fail");
						refresh();
					}
					else if(retc == 0) 
					{
						mvaddstr(22, 4, "                                           ");
						mvaddstr(22, 4, " copy photo successful");
						refresh();
					}
					noecho();
					break;
					//播放音乐失败
			case '4':
					echo();  
					mvaddstr(14, 4, "                                                           ");
					mvaddstr(14, 4, "input music name:");
					refresh();
					while(1)
					{
						scanw("%s", name_music);
						break;
					}
					 int reto1 = play_music(head_music1, name_music);
					 if(reto1 == -1)
					 {
						 //提示播放失败
						 mvaddstr(22, 4, "                                           ");
						 mvaddstr(22, 4, "sorry ! play music fail");
						 refresh();
					 }
					 else 
					 {
						 sleep(1);
						 print();
						 print_node_content_photo(head_photo1);
						 print_node_content_music(head_music1);
						 mvaddstr(22, 4, "                                           ");
						 mvaddstr(22, 4, " play music successful");
						 refresh();
					 }
					 noecho();
					 break;
					 //删除音乐
			case '5':	
					echo();
					mvaddstr(14, 4, "                                                    ");
					mvaddstr(14, 4, "input music name:");
					refresh();
					while(1)
					{
						scanw("%s", name_music);
						break;
					}
					 int reto2 = delete_music(head_music1, name_music);
					 if(reto2 == -1)
					 {
						 //提示失败
						 mvaddstr(22, 4, "                                               ");
						 mvaddstr(22, 4, "sorry ! delete music fail");
						 refresh();
					 }
					 else 
					 {
						print();
						mvaddstr(22, 4, "                                                        ");
						mvaddstr(22, 4, "delete music successful");
						delete_node(head_music1, name_music);
						print_node_content_music(head_music1);
						print_node_content_photo(head_photo1);
						refresh();
					 }
					 noecho();
					 break;
					 //复制音乐文件
			case '6':
					echo();
					mvaddstr(14, 4, "                                                               ");
					mvaddstr(14, 4, "input music name and dest path:");
					refresh();
					while(1)
					{
						scanw("%s%s", name_music, path_music);
						break;
					}
					 int reto3 = copy_music(head_music1, name_music, path_music);
					 if(reto3 == -1)
					 {
						 //提示失
						 mvaddstr(22, 4, "                                                         ");
						 mvaddstr(22, 4, "sorry ! copy music fail");
						 refresh();
					 }
					 else 
					 {
						 mvaddstr(22, 4, "                                                       ");
						 mvaddstr(22, 4, "copy music successful");   
						 refresh();
					 }
					 noecho();
					 break;
					 //最新添加的自动播放模式
			case 'a':
					//清除屏幕
					 clear();
					 refresh();
					 //进入自动模式,如果没有收到信号则一直循环
					 auto_model(head_music1, head_photo1);
					 clear();
					 //恢复原来的界面
					 print();
					 print_node_content_music(head_music1);
					 print_node_content_photo(head_photo1);
					 refresh();
					 sleep(1);
					 break;
			case 'q':
			case 'Q':flag = 1;break;
			default:break;
		}
		if(flag == 1)
			break;
		
	}while(1);
	
	umount_usb();
	
	endwin();
}