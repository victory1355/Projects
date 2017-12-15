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
//用户交互
void key_control()
{
	char name_music[200];
	char name_photo[200];
	char path_photo[200];
	char path_music[200];
	int flag = 0;
	int key;  //阻塞模式
	do
	{
		key = getch();
		switch()
		{
			case '1':mvaddstr(19, 4, "input photo name:");
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
					 break;
			case '2':mvaddstr(19, 4, "input photo name:");
					while(1)
					{
						scanw("%s", name_photo);
						break;
					}
					 int retd = delete_photo(head_photo, name_photo);
					 if(retd == -1)
					 {
						 //提示删除图片失败
						 mvaddstr(22, 4, "sorry ! delete photo fail");
					 }
					 break;
			case '3':mvaddstr(19, 4, "input photo name and dest path:");
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
					break;
			case '4':mvaddstr(19, 4, "input music name:");
					while(1)
					{
						scanw("%s", name_music);
						break;
					}
					 int reto = open_music(head_music, name_music);
					 if(reto == -1)
					 {
						 //提示打开图片失败
						 mvaddstr(22, 4, "sorry ! play music fail");
					 }
					 break;
			case '5':mvaddstr(19, 4, "input music name:");
					while(1)
					{
						scanw("%s", name_music);
						break;
					}
					 int reto = delete_music(head_music, name_music);
					 if(reto == -1)
					 {
						 //提示打开图片失败
						 mvaddstr(22, 4, "sorry ! delete music fail");
					 }
					 break;
			case '6':mvaddstr(19, 4, "input music name and dest path:");
					while(1)
					{
						scanw("%s%s", name_music, path_music);
						break;
					}
					 int reto = delete_music(head_music, name_music, path_music);
					 if(reto == -1)
					 {
						 //提示打开图片失败
						 mvaddstr(22, 4, "sorry ! copy music fail");
					 }
					 break;
			case 'q':
			case 'Q':flag = 1;break;
			default:break;
		}
		if(flag == 1)
			break;
		
	}while(1);
}