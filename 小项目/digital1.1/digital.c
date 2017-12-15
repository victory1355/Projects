#include "digital.h"

int main(int argc, char *argv[])
{
	//测试代码
	//创建头节点
	File_info *head_photo = create_file_head();
	if(head_photo == NULL)
	{
		//perror("create fail");
		return -1;
	}
	File_info *head_music = create_file_head();
	if(head_music == NULL)
	{
		//perror("create fail");
		return -1;
	}
	flag_usb = 10;
	//初始化终端
	init_term();
	
	//创建子进程
	pid_t pid = fork();
	if(pid == -1)
	{
		//printf("fork fail\n");
		return -1;
	}
	//主父进程负责挂载成功的数据处理
	if(pid > 0)
	{
		int stat_loc1;
		//等待子进程的退出
		//printf("tree4\n");
		waitpid(pid, &stat_loc1, WUNTRACED);    //阻塞等待
		//printf("tree5\n");
		
		//提取MP3和JPG图片
		tree_find_file("/mnt/wine", head_photo, head_music);
		
		//打印链表内容
		print();
		print_node_content_music(head_music);
		print_node_content_photo(head_photo);	
		//用户交互
		key_control(head_music ,head_photo);
		
		//printf("tree6\n");
		//进程退出
		exit(0);
	}
	//主子进程
	if(pid == 0)
	{
		pid_t pid_c = fork();
		
		if(pid_c == -1)
		{
			//printf("fork fail\n");
			//mvaddstr(22, 4, "fork fail");
			//refresh();
			return -1;
		}
		//负责不断检测U盘和挂载U盘
		if(pid_c == 0)
		{
			//printf("222\n");
			signal(SIGALRM, found_usb);
			while(1)
			{
				//挂载成功后退出程序
				if(flag_usb == 10)
					raise(SIGALRM);
				//定时检测
				sleep(5);	
				if(flag_usb == 11)
				{
					break;
				}
			}	
			//printf("4353\n");
			//退出子进程
			exit(0);
		}
		//挂载成功后负责打印内容
		if(pid_c > 0)
		{
			int stat_loc2;
			//等待子进程的退出
			waitpid(pid_c, &stat_loc2, WUNTRACED);      //阻塞等待
			exit(0);
			
		}
	}

	return 0;
}






















