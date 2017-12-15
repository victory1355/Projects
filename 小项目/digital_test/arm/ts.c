#include <stdio.h>	
#include <linux/input.h>	//输入子系统

#include <sys/stat.h>//IO
#include <sys/types.h>
#include <fcntl.h>
#include <strings.h>	//字符串

#include <stdbool.h>

#include "ts.h"
static int ts_fd;
int ts_open()
{
	/* 打开TS设备，以可读可写方式打开 */
	ts_fd = open("/dev/event0",O_RDWR);
	
	if(ts_fd < 0)
	{
		perror("open ts fail");
		return -1;
	}
	return 0;
}

void ts_close()
{
	/* 关闭TS设备 */
	close(ts_fd);		
}

/*
	ts_get_xy：用于从触摸屏检测按下的坐标，并且是手指松开后，才会结束
		x：按下时的x坐标
		y：按下时的y坐标
*/
int ts_get_xy(int *ts_x,int *ts_y)
{
	struct input_event ts_buf;
	bzero(&ts_buf, sizeof(ts_buf));
	/*获取触摸屏的触控坐标*/
	while(1)
	{
		read(ts_fd,&ts_buf,sizeof(struct input_event));
		if(ts_buf.type == EV_ABS) //触摸事件
		{
			if(ts_buf.code == ABS_X)
				  *ts_x = ts_buf.value; //x坐标
			if(ts_buf.code == ABS_Y)
				  *ts_y = ts_buf.value; //y坐标	
			//if(ts_buf.code == ABS_PRESSURE && ts_buf.value == 0) //触摸抬起
			//   break;
		}
		//检测手指松开
		if(ts_buf.type==EV_KEY)
		{
			if(ts_buf.code==BTN_TOUCH && ts_buf.value==0)
				break;
		}
	}
	return 0;
}


/*
	wait_ts_touch：用于等待手指按下触摸屏。与wait_ts_leave连用
		x：按下时的x坐标
		y：按下时的y坐标
*/
void wait_ts_touch(int *x,int *y)
{
	struct input_event ts_buf;
	bool x_ready = false;
	bool y_ready = false;
	bzero(&ts_buf, sizeof(ts_buf));
	while(1)
	{
		read(ts_fd,&ts_buf,sizeof(struct input_event));

		if(ts_buf.type == EV_ABS && ts_buf.code == ABS_X)
		{
			*x = ts_buf.value; //x坐标
			x_ready = true;
		}
				 
		if(ts_buf.type == EV_ABS &&ts_buf.code == ABS_Y)
		{
			*y = 479-ts_buf.value; //y坐标
			y_ready = true;
		}
			
		if(x_ready && y_ready)
			break;
	}
}

/*
	wait_ts_leave:用于等待手指离开触摸屏，与wait_ts_touch连用
*/
void wait_ts_leave()
{
	struct input_event buf;

	while(1)
	{
		bzero(&buf, sizeof(buf));
		read(ts_fd, &buf, sizeof(buf));

		if(buf.type == EV_ABS &&
		   buf.code == ABS_PRESSURE &&
		   buf.value == 0)
		{
			break;
		}
	}
}


/*
	get_finger_move_direction:用来获取手指的滑动方向的
		MOVE_UP ,
		MOVE_DOWN,
		MOVE_RIGHT,
		MOVE_LEFT,
*/
int get_finger_move_direction()
{
	int mv;
	struct input_event ev;
	int x1, y1; //滑动过程中第一个点的坐标值
	int x2,y2; //滑动过程中最后一个点的坐标值


	x1 = -1; 
	y1 = -1;
	
	while (1)
	{
		int r = read(ts_fd, &ev, sizeof(ev));
		if (r == sizeof(ev))
		{
			if (ev.type == EV_ABS && ev.code == ABS_X)
			{
				if (x1 == -1)
					x1 = ev.value;
				x2 = ev.value;
			}

			if (ev.type == EV_ABS && ev.code == ABS_Y)
			{
				if (y1 == -1)
					y1 = 479-ev.value;
				y2 = 479-ev.value;
			}

			//手指弹起
			if (ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value == 0)
			{
				int delta_x = abs(x2-x1);
				int delta_y = abs(y2-y1);

				if (delta_x >2*delta_y)
				{
					if (x2 > x1)
					{
						mv = MOVE_RIGHT;
					}
					else
					{
						mv = MOVE_LEFT;
					}
					break;
					
				} else if (delta_y > 2 * delta_x)
				{
					if (y2 > y1)
					{
						mv = MOVE_DOWN;
					}
					else
					{
						mv = MOVE_UP;
					}
					break;
				}
				else //恭喜您，再来一次
				{
					x1 = -1;
					y1 = -1;
				}
			}
		}
	
	}

	return mv;
}
