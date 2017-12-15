#include "ui.h"


//刷新终端的内容列表

void refresh_screen(File_info *head)
{
	//清屏
	clear();
	//打印界面
	//打印链表内容
	refresh();
}
//打开图片
int open_photo(File_info *head , char *file_name)
{
	//遍历查找对应的文件,删除指定目录的文件
	char path[MAX_PATH];
	int ret = search_by_name(head, file_name, path);
	printf("path:%s\n", path);
	if(ret == 0)
	{
		char com[100];
		//打开图片的命令
		sprintf(com, "rm -rf %s", path);
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
	printf("path:%s\n", path);
	if(ret == 0)
	{
		char com[100];
		//播放音乐的命令
		sprintf(com, "rm -rf %s", path);
		system(com);
		return 0;
	}
	else {
		return -1;
	}
}
//删除音乐，输入音乐文件名
int  delete_music(File_info *head, char *file_name)
{
	//遍历查找对应的文件,删除指定目录的文件
	char path[MAX_PATH];
	int ret = search_by_name(head, file_name, path);
	printf("path:%s\n", path);
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
	}
}
//打印界面
void print_box()
{
	//整个界面的框架
	refresh();
}
//打印链表内容
void print_node_content(File_info *head)
{
	struct list_head *pos=NULL;
	int line = 4;
	int cols = 4;
	list_for_each(pos, &head->list)
	{
		
		File_info *file = list_entry(pos, File_info, list);
		mvprintw(line, cols, "file name:%s file size:%d",file->file_group.file_name, file->file_group.file_size);
		line++;
		if(line == 19)
		{
			line = 4;
			clos = 20;
		}
		
	}	
}









