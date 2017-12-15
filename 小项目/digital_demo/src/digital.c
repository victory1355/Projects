#include "digital.h"

//两个表头全局变量
File_info *head_photo;
File_info *head_music;


//提取MP3和JPG图片,并建立一条链表
void tree_find_file(char *dir ,  File_info *head_photo, File_info *head_music)
{ 
    //打开目录指针 
    DIR *Dp; 
    //文件目录结构体 
    struct dirent *enty; 
    //详细文件信息结构体 
    struct stat statbuf; 
    //打开指定的目录，获得目录指针 
    if(NULL == (Dp = opendir(dir))) 
    {
		clear();
        mvaddstr(22,4," open dir fail"); 
		refresh();
        return; 
    } 
	else 
	{
		clear();
		mvaddstr(22,4," open successful");
		refresh();
	}
    //遍历这个目录下的所有文件 
	while(enty = readdir(Dp))
	{
		//判断是否为目录
		if(enty->d_type == DT_DIR) 
		{    			
			if(0 == strcmp(".",enty->d_name) || 0 == strcmp("..",enty->d_name)) //当前目录和上一目录过滤掉 
			{ 
				continue; 
			} 
			char path_buf[MAX_PATH];
			char ch = '/';
			char *p = strrchr(dir,ch);
			int flag = strcasecmp(p, "/");
			if(flag ==0)
			{
				sprintf(path_buf, "%s%s", dir, enty->d_name); 
			}
			else 
				sprintf(path_buf, "%s/%s", dir, enty->d_name);
			//继续递归调用        
			tree_find_file(path_buf, head_photo, head_music); 
		} 
		else
		{     
			//判断是不是mp3或者jpg
			char *name = enty->d_name;
			char *p = strrchr(name, '.');
			if(p != NULL)
			{
				if(strcasecmp(p , ".jpg")==0)
				{					
					//创建图片链表
					char path_photo[MAX_PATH];
					char ch_p = '/';
					char *p_p = strrchr(dir,ch_p);
					int flag_p = strcasecmp(p_p, "/");
					if(flag_p ==0)
					{
						sprintf(path_photo, "%s%s", dir, enty->d_name); 
					}
					else 
					sprintf(path_photo, "%s/%s", dir, enty->d_name);
					creat_file_list_photo(head_photo, enty->d_name, enty->d_reclen, path_photo);//出现段错误				
				}
				if(strcasecmp(p , ".mp3")==0)
				{					
					//创建MP3链表
					char path_music[MAX_PATH];
					char ch_m = '/';
					char *p_m = strrchr(dir,ch_m);
					int flag_m = strcasecmp(p_m, "/");
					if(flag_m ==0)
					{
						sprintf(path_music, "%s%s", dir, enty->d_name); 
					}
					else 
					sprintf(path_music, "%s/%s", dir, enty->d_name);					
					creat_file_list_music(head_music, enty->d_name, enty->d_reclen, path_music);		
				}	
			}
		}
	}  
    closedir(Dp); 
	return ;
} 




//创建表头
File_info * create_file_head()
{
	
		File_info *head =(File_info *) malloc(sizeof(File_info));
		if(head == NULL)
			return NULL;
		INIT_LIST_HEAD(&head->list);
		return head;
}
//创建图片信息链表
int creat_file_list_photo(File_info *head, char *name, int size, char *path)
{
		
	File_info * node = (File_info *)malloc(sizeof(File_info));
	if(node == NULL)
		return -1;
	INIT_LIST_HEAD(&node->list);	
	node->file_group.file_size = size;
	strcpy(node->file_group.file_name, name);
	strcpy(node->file_group.path, path);
	//插入表头	
	list_add(&node->list, &head->list);
	return 0;
}
//创建音乐信息链表
int creat_file_list_music(File_info *head, char *name, int size, char *path)
{
		
	File_info * node = (File_info *)malloc(sizeof(File_info));
	if(node == NULL)
		return -1;
	INIT_LIST_HEAD(&node->list);
	
	node->file_group.file_size = size;
	strcpy(node->file_group.file_name, name);	
	strcpy(node->file_group.path, path);
	//插入表头	
	list_add(&node->list, &head->list);    //出现段错误	
	return 0;
}

//遍历链表,使用回调函数，多次使用
void travel_list(File_info *head)
{
	//printf("flag4\n");
	struct list_head *pos=NULL;
	list_for_each(pos, &head->list)
	{
		//printf("flag5\n");
		File_info *file = list_entry(pos, File_info, list);
		printf("name:%s size:%d path:%s\n", file->file_group.file_name, file->file_group.file_size, file->file_group.path);
	}
	//printf("flag6\n");//段错误的地方
	
}
//打印文件信息
void print_file_info(File_info *node)
{
	//直接打印文件的内容，使用curses库
	printf("name:%s size:%d path:%s\n", node->file_group.file_name, node->file_group.file_size, node->file_group.path);
}

//通过名字查找对应的文件的绝对路径
int  search_by_name(File_info *node, char *name, char *return_path) 
{
	//查找指定名字的文件的绝对路
	struct list_head *pos = NULL;
	
	list_for_each(pos, &node->list)
	{
		File_info *file = list_entry(pos, File_info, list);	  
		int ret = strcmp(file->file_group.file_name, name);		
		if(ret == 0)
		{
			//复制绝对路径
			strcpy(return_path, file->file_group.path);
			return 0;
		}
	}
	
	return -1;
}

//删除图片
int  delete_photo(File_info *head, char *file_name)
{
	//遍历查找对应的文件,删除指定目录的文件
	if(head == NULL)
		return -1;
	char path[MAX_PATH];
	
	int ret = search_by_name(head, file_name, path);
	
	refresh();
	//printf("path:%s\n", path);
	if(ret == 0)
	{
		char com[100];
		sprintf(com, "rm -rf %s", path);
		system(com);
		return 0;
	}
	else {
		return -1;
	}
	//刷新终端的内容列表
}
//删除节点
int  delete_node(File_info *head, char *name)
{
	//查找指定名字的文件的绝对路径，如果不存在就提示用户,并返回1
	struct list_head *pos;
	if(head == NULL)
		return -1;
	list_for_each(pos, &head->list)
	{
		File_info *file = list_entry(pos, File_info, list);
		//如果文件存在就复制
		int ret = strcmp(file->file_group.file_name, name);
		if(ret == 0)
		{
			list_del(&file->list);
			return 0;
		}
		
	}
	return -1;
}

//复制图片到指定的目录,成功返回0，失败返回-1
int  copy_photo(File_info *head, char *name, char *dest_path)
{
	//查找指定名字的文件的绝对路径，如果不存在就提示用户,并返回1
	struct list_head *pos;
	if(head == NULL)
		return -1;
	list_for_each(pos, &head->list)
	{
		File_info *file = list_entry(pos, File_info, list);
		//如果文件存在就复制
		int ret = strcmp(file->file_group.file_name, name);
		
		if(ret == 0)
		{
				//通过io操作复制文件
				int retc = copy(file->file_group.path ,dest_path);
				if(retc == 0)
				{
					return 0;
				}
				else
				{
					return -1;
				}
					
		}
	}
}
//文件复制的接口,成功返回0，失败返回-1
int copy(char *src, char *dest)
{
	FILE *p1, *p2;
	char buf[128] = {0};
	p1 = fopen(src, "r+");
	p2 = fopen(dest, "w+");	
	if(p2 == NULL)
	{
		return -1;
	}
	while(fread(buf, sizeof(char),sizeof(buf), p1) != 0)
	{
		fwrite(buf, sizeof(char), sizeof(buf), p2);
		//清空缓冲区
		bzero(buf, sizeof(buf));
	}
	fclose(p1);
	fclose(p2);
	return 0;
}
//删除音乐，输入音乐文件名
int  delete_music(File_info *head, char *file_name)
{
	//遍历查找对应的文件,删除指定目录的文件
	char path[MAX_PATH];
	int ret = search_by_name(head, file_name, path);
	if(ret == 0)
	{
		char com[100];
		sprintf(com, "rm -rf %s", path);
		system(com);
		return 0;
	}
	else {
		return -1;
	}
	//刷新终端的内容列表
}

//复制音乐到指定的目录,文件名+绝对路径，成功返回0，失败返回-1
int  copy_music(File_info *head, char *name, char *dest_path)
{
	//查找指定名字的文件的绝对路径，如果不存在就提示用户,并返回1
	if(head == NULL)
		return -1;
	struct list_head *pos;
	list_for_each(pos, &head->list)
	{
		File_info *file = list_entry(pos, File_info, list);
		//如果文件存在就复制
		int ret = strcmp(file->file_group.file_name, name);
		if(ret == 0)
		{
				//通过io操作复制文件
				int retc = copy(file->file_group.path ,dest_path);
				if(retc == 0)
				{
					return 0;
				}
				else
				{
					return -1;
				}			
		}
		//提示用户该文件不存在
	}
}




//打开图片
int open_photo(File_info *head , char *file_name)
{
	//遍历查找对应的文件,删除指定目录的文件
	char path[MAX_PATH];
	int ret = search_by_name(head, file_name, path);

	if(ret == 0)
	{
		char com[100];
		//打开图片的命令
		sprintf(com, "viewnior %s ", path);
		system(com);
		
		return 0;
	}
	else {
		return -1;
	}
}

//播放音乐,输入音乐名，成功返回0，失败返回1
int play_music(File_info *head ,char *file_name)
{
	char path[MAX_PATH];
	int ret = search_by_name(head, file_name, path);
	if(ret == 0)
	{
		char com[100];
		//播放音乐的命令
		sprintf(com, "madplay %s &", path);
		system(com);
		return 0;
	}
	else {
		return -1;
	}
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
			
			case 'W':
			case 'w':system("killall -9 madplay");
						mvaddstr(22, 4, "                                           ");
						 mvaddstr(22, 4, "close music successful");
						 refresh();
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
//自动挂载U盘
int usb_mount()
{
	DIR *dr;
	
	FILE *fd;
	char b[50];
	char usb[10]= "/dev/sd";
	char find[50]={0};
	char type[50]={0};
	while(1)
	{
		sprintf(b,"fdisk -l > /mnt/hgfs/workspace/test.txt ");
		system(b);
		
		fd = fopen("/mnt/hgfs/workspace/test.txt","r+");
		
		while(!feof(fd))
		{
			
			fscanf(fd,"%s",find);
			
			while(find[5] == 's'&&find[6]== 'd'&&find[7] != 'a'&&find[8] != ':')
			{
					
					strcat(usb,&find[7]);				
					sprintf(b,"umount %s  ",usb);
					printf("%s",usb);
					system(b);
					while(!feof(fd))
					{
						fscanf(fd,"%s\n",type);
						if(type[5]=='N'&&type[6]=='T')
						{
							sprintf(b,"mount -t ntfs-3g %s /mnt/usb  ",usb);
							break;
						}
								
					}
					if(feof(fd))
					sprintf(b,"mount -t vfat %s /mnt/usb  ",usb);				
					system(b);
					printf("successful");
					return 0;
			}
		}		
					
		printf("no found!\n");
		sleep(2);	
	}
	 
	fclose(fd);
	return 0;
	
}

int main(int argc, char *argv[])
{
	
	usb_mount();
	//测试代码
	
	//创建头节点
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
	
	//提取MP3和JPG图片
	tree_find_file("/mnt/usb", head_photo, head_music);
	
	//初始化终端
	init_term();
	
	//打印链表内容
	print_node_content_music(head_music);
	print_node_content_photo(head_photo);
	
	//用户交互
	key_control(head_music ,head_photo);
	return 0;
}

