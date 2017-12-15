#include <stdio.h>   	//printf scanf
#include <fcntl.h>		//open write read lseek close  	 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>

//音乐的播放，参数为要播放的文件名
void music_play(const char *pmusic_path) 
{
	char cmd_buf[64]={0};

	sprintf(cmd_buf,"madplay %s & \n",pmusic_path);

	if(0 == vfork())
	{
		system(cmd_buf);
		exit(0);
	}		
	
	
}

void music_pause(void) //音乐暂停
{
	system("killall -STOP madplay &");	
	
}

void music_continue(void)  //音乐继续
{
	
	system("killall -CONT madplay &");	
	
}

void music_stop(void)		//音乐的停止
{
	
	system("killall madplay \n");		
}

//视频播放，参数为要播放的文件名
void video_play(const char *video_path)  
{
	char cmd_buf[64]={0};

	sprintf(cmd_buf,"mplayer %s & \n",video_path);

	
	if(0 == vfork())
	{
		system(cmd_buf);	
		exit(0);
	}	
}

void vedio_pause(void)  //视频暂停
{
	system("killall -STOP mplayer &");	
	
}

void vedio_continue(void) //视频继续
{
	
	system("killall -CONT mplayer &");	
	
}

void vedio_stop(void)		//视频的停止
{
	
	system("killall mplayer \n");		
}