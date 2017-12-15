#include <stdio.h>
#include <pthread.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define DEVICE_NAME "/dev/s3c2410_serial1"

#include "common.h"
#include "lcd.h"
#include "ts.h"
#include "vedio.h"

int ser_fd;

int uart_open()
{
	//打开串口
	ser_fd = open(DEVICE_NAME,O_RDWR);
	if(ser_fd == -1)
	{
		printf("serical open failed!\n");
		return -1;
	}
	//初始化，配置串口
	set_serial_uart1(ser_fd);
	
}

void hardware()
{
	int x=0,y=0;
	printf("hardware ioctl\n");
	lcd_draw_bmp(0,0,"hardware.bmp");
	
	while(1)
	{
		ts_get_xy(&x,&y);
		if(x>90&& x<200 &&y>180 &&y<280)//LED1 ON 黄灯亮
			write(ser_fd,"11",3);	
		else if(x>160&& x<280 &&y>320 &&y<430)//LED1 OFF 黄灯灭
			write(ser_fd,"10",3);
		else if(x>480&& x<580 &&y>160 &&y<270)//LED2 ON 蓝灯亮
			write(ser_fd,"21",3);
		else if(x>480&& x<580 &&y>320 &&y<430)//LED2 OFF 蓝灯灭
			write(ser_fd,"20",3);
		else if(x>700&&y>400) //返回按键
			break;
	}
	lcd_draw_bmp(0,0,"choose_fun.bmp"); //返回到功能选择界面
}

void play_v()
{
	int x=0,y=0;
	printf("play_v ioctl\n");
	lcd_draw_bmp(0,0,"hardware.bmp");
	
	while(1)
	{
		ts_get_xy(&x,&y);
		if(x>160&& x<280 &&y>160 &&y<270)//视频播放
			video_play("mv.mp4");
		else if(x>160&& x<280 &&y>320 &&y<430)//视频暂停
			vedio_pause();
		else if(x>480&& x<580 &&y>160 &&y<270)//视频继续
			vedio_continue();
		else if(x>480&& x<580 &&y>320 &&y<430)//视频的停止
			vedio_stop();
		else if(x>700&&y>400) //返回按键
			break;
	}
	lcd_draw_bmp(0,0,"choose_fun.bmp"); //返回到功能选择界面
}

void play_s()
{
	int x=0,y=0;
	printf("play_s ioctl\n");
	lcd_draw_bmp(0,0,"hardware.bmp");
	
	while(1)
	{
		ts_get_xy(&x,&y);
		if(x>160&& x<280 &&y>160 &&y<270)//音乐播放
			music_play("1.mp3");
		else if(x>160&& x<280 &&y>320 &&y<430)//音乐暂停
			music_pause(); 
		else if(x>480&& x<580 &&y>160 &&y<270)//音乐继续
			music_continue();
		else if(x>480&& x<580 &&y>320 &&y<430)//音乐的停止
			music_stop();
		else if(x>700&&y>400) //返回按键
			break;
	}
	lcd_draw_bmp(0,0,"choose_fun.bmp"); //返回到功能选择界面
}


int main()
{
	int x=0,y=0;
	//相关初始化
	//屏幕初始化
	lcd_open();  //lcd.c
	//触摸屏初始化
	ts_open();  //ts.c
	//串口初始化
	uart_open();  //main.c
	//串口配置
	serical_ioctl(ser_fd);

	//1.显示登录界面
	lcd_draw_bmp(0,0,"meun.bmp");  
	sleep(2);
	
	//2.显示功能选择界面
	lcd_draw_bmp(0,0,"choose_fun.bmp");  
	
	//3.进入功能选择
	while(1)
	{
		ts_get_xy(&x,&y); //ts.c
		printf("x1=%d,y1=%d\n",x,y);
		
		//点击触摸屏不同区域，实现不同功能
		if(x>400&& x<500 &&y>66 &&y<150)//灯光控制界面
			hardware();
		else if(x>550&& x<650 &&y>66 &&y<150) //音乐
			play_s();
		else if(x>400&& x<500 &&y>200 &&y<280)	//视频
			play_v();
		else if(x>700&&y>400) //返回
			break;
	}
		
	lcd_draw_bmp(0,0,"ui/byebye.bmp");  //退出界面
	
	//解除初始化
	lcd_close();  //lcd.c
	ts_close();  //ts.c
	close(ser_fd);
}