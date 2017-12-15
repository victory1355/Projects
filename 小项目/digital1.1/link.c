#include "digital.h"

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
		mvaddstr(22,4," open dir successful");
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







