#include <stdio.h>   	//printf scanf
#include <fcntl.h>		//open write read lseek close  	 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>

//���ֵĲ��ţ�����ΪҪ���ŵ��ļ���
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

void music_pause(void) //������ͣ
{
	system("killall -STOP madplay &");	
	
}

void music_continue(void)  //���ּ���
{
	
	system("killall -CONT madplay &");	
	
}

void music_stop(void)		//���ֵ�ֹͣ
{
	
	system("killall madplay \n");		
}

//��Ƶ���ţ�����ΪҪ���ŵ��ļ���
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

void vedio_pause(void)  //��Ƶ��ͣ
{
	system("killall -STOP mplayer &");	
	
}

void vedio_continue(void) //��Ƶ����
{
	
	system("killall -CONT mplayer &");	
	
}

void vedio_stop(void)		//��Ƶ��ֹͣ
{
	
	system("killall mplayer \n");		
}