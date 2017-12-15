#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

//对串口进行配置（波特率、流量控制、帧格式（数据位、停止位、校验位））
int set_serial_uart1(int ser_fd)
{
	//声明设置串口的结构体
	struct termios termios_new;
	//先清空该结构体
	bzero( &termios_new, sizeof(termios_new));	
	//cfmakeraw()设置终端属性，就是设置termios结构中的各个参数。（原始模式  RAW模式）
	cfmakeraw(&termios_new);

	//设置波特率为115200
	termios_new.c_cflag=(B115200);
	//CLOCAL和CREAD分别用于本地连接和接受使能，因此，首先要通过位掩码的方式激活这两个选项。 
	termios_new.c_cflag |= CLOCAL | CREAD;
	
	//通过掩码设置数据位为8位
	termios_new.c_cflag &= ~CSIZE;
	termios_new.c_cflag |= CS8;

	//设置无奇偶校验
	termios_new.c_cflag &= ~PARENB;

	//一位停止位
	termios_new.c_cflag &= ~CSTOPB;
	//清除串口缓冲区（刷新缓冲区）
	tcflush( ser_fd,TCIOFLUSH);
	// 可设置接收字符和等待时间，无特殊要求可以将其设置为0
	termios_new.c_cc[VTIME] = 0; //读取不等待
	termios_new.c_cc[VMIN] = 4; //读取的最少字符
	// 用于清空输入/输出缓冲区
	tcflush ( ser_fd, TCIOFLUSH);
	//完成配置后，可以使用以下函数激活串口设置
	if(tcsetattr( ser_fd ,TCSANOW,&termios_new))  //串口设置使能
	printf("Setting the serial1 failed!\n");
}

void serical_ioctl(int ser_fd)
{
	if(fcntl(ser_fd,F_SETFL,0)<0)	
	{
		exit(1);
	}
	tcdrain(ser_fd);  //通用终端控制
	tcflush( ser_fd, TCIOFLUSH);  //刷新缓冲区
}


