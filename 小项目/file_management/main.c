#include "management.h"


int main()
{	
		
		//创建头结点
		//路径链表头结点
		struct list_head path_head;
		//文件信息链表头结点
		struct list_head filename_head;
		create_head(&path_head, &filename_head);
		
		//创建当前目录下的文件信息链表
	    update_file_info("ls -l |awk 'NR!=1{print $1,$5,$9}'>   /mnt/hgfs/workspace/file_info.txt");
		 //创建文件信息链表
		get_file_info(&filename_head);
		//获取当前路径,并建立路径表头
		get_first_path(&path_head);
		//打印当前路径
		show_path_node(&path_head);
		
		getchar();
		
		int tmp;
		//进入主界面
		display(&path_head);
		while(1)
		{
				printf("请按数字键选择不同的功能\n");
				scanf("%d", &num);
				if(num == 1)
				{
					printf("你当前处于根目录，不可以返回上一级\n");
				}
				else 
					break;
		}
	
		
		while(1)
		{
			//返回上一级目录
			if(num == 1)
			{ 
				back(&path_head, &filename_head);
			}
			//进入下一级目录
			if(num == 2)
			{
				printf("温馨提示：以------根目录/当前目录-----格式进入你想去世界\n");
				tmp = show_doc(&path_head, &filename_head);
				if(tmp == 1)
				{
					printf("温馨提示：当前目录下不存在子目录......\n");
					scanf("%d", &num);
				}
				else{
					scanf("%s", doc);
					while(getchar()!= '\n');
					next(&path_head, &filename_head);
				}
			}
			//显示文件内容
			if(num == 3)
			{
				show_file(&filename_head);
			}
			//修改文件名
			if(num == 4)
			{
				change_name();
			}
			if(num == 5)
			{
				//退出并销毁链表
				quit(&path_head, &filename_head);
				break;
			}
				
		}
		
		return 0;
}


