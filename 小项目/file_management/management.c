#include "management.h"


//功能函数的具体实现


//创建头结点
void create_head(struct list_head *path_head, struct list_head *filename_head)
{
	INIT_LIST_HEAD(path_head);
	INIT_LIST_HEAD(filename_head);
}
//上一级目录
void back(struct list_head *path_head, struct list_head *filename_head)
{
	//display();
	char commond[300] = "ls -l                                                   |awk 'NR!=1{print $1,$5,$9}' >/mnt/hgfs/workspace/file_info.txt";
	//销毁旧的文件信息
	destroy(filename_head);
	//删除最后一个路径结点
	list_del_init(path_head->prev);
	//printf("flag\n");
	struct list_head *pos = path_head->prev;
	Path_node *pf = list_entry(pos, Path_node, list);
	//printf("pf->path = %s\n", pf->path);
	//更新目录文件信息文档
	int i = 0;
	while(pf->path[i] != '\0')
	{
		commond[strlen("ls -l ")+i] = pf->path[i];
		i++;
	}
	//printf("com = %s\n", commond);
	update_file_info(commond);
	
	//创建文件信息链表
	get_file_info(filename_head);
	show_path_node(path_head);
	//打印上一级目录的信息
	scanf("%d", &num);
	printf("请按数字键选择不同的功能\n");

}
//进入子目录

void next(struct list_head *path_head, struct list_head *filename_head)
{
	char commond[300] = "ls -l /mnt/hgfs/workspace/file_management                     |awk 'NR!=1{print$1,$5,$9}' > /mnt/hgfs/workspace/file_info.txt";                    
	//printf("%s\n", commond);
	int i = 0;
	while(doc[i] != '\0')
	{
		commond[strlen("ls -l /mnt/hgfs/workspace/file_management")] = '/';
		commond[strlen("ls -l /mnt/hgfs/workspace/file_management/")+i] = doc[i];
		i++;
	}
	//更新文件信息文档
	update_file_info(commond);
	destroy(filename_head);
	//创建路径链表
	get_path(path_head, filename_head);
	//show_doc(path_head, filename_head);
	show_path_node(path_head);
	//清空目录
	for(i=0; i<200; i++)
		doc[i] = "";
	//printf("commond = %s\n", commond);
	//显示主界面
	//display(path_head);
	
	
	//创建文件信息链表
	get_file_info(filename_head);
	
	//切换到其他模块
	printf("请按数字键选择不同的功能\n");
	scanf("%d", &num);
	
	//getchar();
	//show_file_node(filename_head);
}
//查看文件信息
void show_file(struct list_head *filename_head)
{
	//遍历文件链表结点信息
	show_file_node(filename_head);
	//printf("查看文件信息\n");
	printf("请按数字键选择不同的功能\n");
	scanf("%d", &num);
	
	//printf("num == %d\n", num);
}
//修改文件名
void change_name()
{
	//修改文件链表结点信息
	printf("修改文件信息\n");
	printf("请按数字键选择不同的功能\n");
	scanf("%d", &num);

}

//退出
void quit(struct list_head *path_head, struct list_head *filename_head)
{
	//销毁两条链表
	//销毁链表，但是保留头结点
	int a;
	a = destroy(path_head);
	if(a == 0)
	{
		printf("链表已销毁\n");
	}
	//销毁旧的文件信息
	destroy(filename_head);
	printf("退出......\n");
}

//路径链表添加结点
void add_path_node(struct list_head *new, struct list_head *head)
{
	list_add_tail(new, head);
}
//文件信息链表添加结点
void add_file_node(struct list_head *new, struct list_head *head)
{
	list_add_tail(new, head);
}

//更新文件信息
void update_file_info(char *p)
{
	system(p);
}
//获取文件信息,并建立文件信息链表
int get_file_info(struct list_head *filename_head)
{
	FILE *fp;
	int  i = 0, lenght;
	char buffer_type[100][100];
	char buffer_size[100][100];
	char buffer_name[100][100];
	fp = fopen("/mnt/hgfs/workspace/file_info.txt","r");
	if(fp == NULL)
	{
			printf("打开信息文件失败\n");
			return -1;
	}
	while(fscanf(fp, "%s%s%s", buffer_type[i], buffer_size[i], buffer_name[i]) != EOF)
	{
		i++;
	}
	lenght = i;
	for(i=0; i<lenght; i++)
	{
		//创建文件信息链表
		File_node * file_info = (File_node *)malloc(sizeof(File_node));
		//填写文件信息
		strcpy(file_info->name, buffer_name[i]);
		strcpy(file_info->type, buffer_type[i]);
		strcpy(file_info->size, buffer_size[i]);
		
		//初始化小结构体
		INIT_LIST_HEAD(&file_info->list);
		//add_path_node(&document_name->list ,&path_head);
		add_file_node(&file_info->list, filename_head);
		//打印下一级目录的信息
	}
}

//遍历并打印文件信息
void show_file_node(struct list_head *filename_head)
{
	struct list_head *pos, *p_head = filename_head;
	if(filename_head->next != filename_head->prev)
		printf("文件名                   文件类型              文件大小\n");
	else
		printf("该目录下没有文件..................\n");
	
	list_for_each(pos, p_head)
	{
		File_node *pf = list_entry(pos, File_node, list);
		printf("%-24s%-24s%-24s\n", pf->name, pf->type, pf->size);
	}
	
	printf("\n");
}


//遍历并打印路径信息
void show_path_node(struct list_head *path_head)
{
	struct list_head *pos = path_head->prev, *p_head = path_head->next;
	//printf("文件名                   文件类型              文件大小\n");
	printf("当前路径：");
	/*
	list_for_each(pos, p_head)
	{
		Path_node *pf = list_entry(pos, Path_node, list);
		printf("/%s", pf->path);
	}*/
	Path_node *pf1 = list_entry(pos, Path_node, list);
	Path_node *pf2 = list_entry(p_head, Path_node, list);
	if(pf1 == pf2)
	{
		printf("%s", pf2->path);
		return ;
	}
	printf("%s", pf2->path);
	printf("/%s", pf1->path);

	printf("\n");
	
	return ;
}


//打印当前目录下的所有子目录
int  show_doc(struct list_head *path_head, struct list_head *filename_head)
{
	struct list_head *pos, *p_head = filename_head;
	int flag = 0;
	//char path[30];
	//判断文件类型
	printf("当前目录下的子目录:");
	list_for_each(pos, p_head)
	{
		File_node *pf = list_entry(pos, File_node, list);
		//printf("pf->type[0] = %c\n", pf->type[0]);
		if(pf->type[0] == 'd')
		{
			printf("%-6s\t", pf->name);
			flag = 1;
		}	
	}
	printf("\n");
	if(flag == 0)
	{
		printf("温馨提示：当前目录下没有子目录!\n");
		return 1;
	}
		
	return 0;
}


//创建路径链表
void get_path(struct list_head *path_head, struct list_head *filename_head)
{
	
	Path_node * p = (Path_node *)malloc(sizeof(Path_node));
	INIT_LIST_HEAD(&p->list);
	
	//Path_node *pf = list_entry(path_head->prev, Path_node, list);
	stpcpy(p->path, doc);
	//添加路径结点
	add_path_node(&p->list, path_head);
	//打印路径
	//show_path_node(path_head);
	return ;
}

//获取当前路径
int get_first_path(struct list_head * path_head)
{
	char path_info[150];
	system("pwd > /mnt/hgfs/workspace/path.txt");
	FILE *fp = fopen("/mnt/hgfs/workspace/path.txt", "r");
	if(fp == NULL)
	{
		printf("打开路径文件失败\n");
		return -1;
	}
	fscanf(fp, "%s", path_info);
	//printf("当前路径:%s\n", path_info);
	Path_node * path_node = (Path_node *)malloc(sizeof(Path_node));
	INIT_LIST_HEAD(&path_node->list);
	strcpy(path_node->path, path_info);
	
	add_path_node(&path_node->list, path_head);
	
	return 0;
}

//主界面函数
void display(struct list_head *path_head)
{
	printf("--------------文件管理器-----------------\n");
	printf("-----------------------------------------\n");
	printf("ps:  通过按数字键选择同的功能\n");
	printf("-----------------------------------------\n\n");
	printf("1：返回上一级目录... 2：进入下一级目录...\n");
	printf("3：查看文件信息      4：修改文件内容\n");
	printf("5: 退出文件管理器------------------------\n\n");
	printf("-----------------------------------------\n");
	show_path_node(path_head);
	printf("-----------------------------------------\n");
}

//销毁链表，但是保留头结点
int  destroy(struct list_head *path_head)
{
	struct list_head *pos, *p_head = path_head, *delete;
	//printf("flag1\n");
	pos = p_head->next;
	while( pos != p_head)
	{
		delete = pos;
		pos = pos->next;
		//printf("flag1\n");
		list_del_init(delete);
	}
	//printf("flag2\n");
	if(path_head->prev == path_head->next)
		return 0;
	else
		return -1;
}










