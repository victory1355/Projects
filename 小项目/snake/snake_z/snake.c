#include"snake.h"

//控制速度
int speed=5,x=0,y=0;
//食物产生的位置
int food_pos_x,food_pos_y;

int speed_time=5;
//吃掉食物的个数
int eat=0;
int Countdown=1;
int times=0,flag=0;
Pos *head;

void create_screen()
{
	initscr();	
	noecho();//关回显
	//cbreak();
	keypad(stdscr,TRUE);
	if(has_colors())
	{
		start_color();
		
		init_pair(0,COLOR_BLACK,COLOR_BLACK);
		init_pair(1,COLOR_GREEN,COLOR_BLACK);
		init_pair(2,COLOR_RED,COLOR_BLACK);
		init_pair(3,COLOR_CYAN,COLOR_BLACK);
		init_pair(4,COLOR_WHITE,COLOR_BLACK);
		init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
		init_pair(6,COLOR_BLUE,COLOR_BLACK);
		init_pair(7,COLOR_YELLOW,COLOR_BLACK);
		
	}
	
	curs_set(FALSE);//取消光标
	attron(COLOR_PAIR(0));
	
	int i,j;
	for(i=0; i<LINES; i++)
	{
		mvaddch(i,0,'|');
		mvaddch(i,COLS-1,'|');
	}
		
	for(j=0; j<COLS; j++)
	{
		mvaddch(0,j,'-');
		mvaddch(LINES-1,j,'-');
		mvaddch(5,j,'-');
	}
	attron(COLOR_PAIR(7));
	mvaddstr(1, 2, "           				GAME: SNAKE        				");
    mvaddstr(2, 2, " 	GAME CONTROL:control direction to move,press '+''-'to change speed");
    mvaddstr(3, 2, " 	GAME REGULATION:win: eat 30. false:strike wall,itself or not eat over 30 time ");
    mvaddstr(4, 2, "  							date:2017/7/30");  
	
	refresh();
}


  
void speed_set()  
{  

    struct itimerval new_value, old_value;  
    new_value.it_value.tv_sec = 0;  
    new_value.it_value.tv_usec = 30000; 
    new_value.it_interval.tv_sec = 0;  
    new_value.it_interval.tv_usec = 30000;  
    setitimer(ITIMER_REAL, &new_value, NULL);     
      
}  


//创建蛇头
void create_snake()
{
	srand(time(NULL));
	head = (Pos *)malloc(sizeof(Pos));
	head->x = 10;
	head->y = 10;
	INIT_LIST_HEAD(&head->list);
	
	insert_tail();
	show_snake(head);
	
	//getch();
	refresh();
}


void insert_tail()
{
	Pos *node = (Pos *)malloc(sizeof(Pos));	
	struct list_head *p = &(head->list);

	//新插入的节点的坐标=头节点的坐标+当前的坐标
	node->x = head->x+x;
	node->y = head->y+y;
	
	//在尾部添加一个节点
	list_add_tail(&node->list,&head->list);

	p = p->prev;
	head = container_of(p,Pos,list);
	attron(COLOR_PAIR(6)|A_BOLD);
	//在新插入的节点的坐标打印一个字符
	mvaddch(head->y,head->x, '0');
	refresh();
}

void game_over()
{
	attron(COLOR_PAIR(4));
	mvaddstr((LINES)/2,(COLS-11)/2,"GAME OVER");
	mvaddstr((LINES+2)/2,(COLS-16)/2,"press 'q' to exit");
	refresh();
}

void snake_move()
{
	if(head->x == COLS-1 || head->x == 0 || head->y == LINES-1 || head->y == 5 || search() || times>1000)
	{
		game_over();
		
		return;
	}
	if( speed>0 && --speed_time <0 )//--代表什么？
	{	
		speed_time = speed;

		if(head->x == food_pos_x && head->y == food_pos_y)
		{
			eat++;
			if(eat >= 30)//??? 
				{
					attron(COLOR_PAIR(4));
					mvaddstr((LINES)/2,(COLS-11)/2," You win");
					mvaddstr((LINES+2)/2,(COLS-16)/2,"press 'q' to exit");
					refresh();
					//sleep(1);//???
					return;//????
				}	
			times = 0;

			food();
			insert_tail();
			flag = 1;
		}
		else
	    {
	    	Pos *q;
	    	struct list_head *p = &(head->list);
	    	p = p->prev;	
	    	q = container_of(p, Pos, list);
			//在尾部打印一个空字符
	    	mvaddch(q->y,q->x,' ');
			
			//删除最后一个节点
	    	list_del(p);
			
			//尾部添加一个节点
			insert_tail();
			
			flag = 1;
	    }
		refresh();
	}

	times++;

	signal(SIGALRM, snake_move); 
}

//从头开始打印整个链表的内容
void show_snake()
{
	
	Pos *p = head;
	struct list_head *l = &head->list;
	attron(COLOR_PAIR(6));
	//先执行再判断，采用do-while()结构
	do
	{
		
		mvaddch(p->y,p->x,'0');
		l = l->next;
		p = container_of(l,Pos,list);
	}while(p != head);
		
	refresh();
}
//产生一个随机数作为坐标，并打印一个字符
 void food()
 {
 	srand(time(NULL));
 	food_pos_x = rand()%(COLS-2)+1;
 	food_pos_y = rand()%(LINES-7)+6;
 	mvprintw(food_pos_y,food_pos_x,"@");
 }
//判断是否撞到自己
 int search()
 {
	 int i=0;
 	Pos *p = head,*q;
	struct list_head *l = &head->list;
	for(i=0; i<5; i++)
	{
		l = l->next;
		p = container_of(l,Pos,list);
		if(p == head)
			return false;
	}	
	
	p = container_of(l,Pos,list);
		
	while(p != head)
	{
		if(p->x == head->x && p->y == head->y)
			return true;
		
		l = l->next;
		p = container_of(l,Pos,list);		
	}
	
	return false;	
 }

 
 
void control()
{
	int KEY = 1;
	//逗号运算符
	while(KEY = getch() , KEY !='q')
	{
		if(flag == 0) continue;
		switch(KEY)
		{
			case KEY_UP:
						if(x == 0 && y == 1) break;	
						x = 0;
						y = -1;
						break;
	
			case KEY_DOWN:
						if(x == 0 && y == -1) break;	
						x = 0;
						y = 1;
						break;

			case KEY_LEFT:
						if(x == 1 && y == 0) break;	
						x = -1;
						y = 0;
						break;

			case KEY_RIGHT:
						if(x == -1 && y == 0) break;	
						x = 1;
						y = 0;
						break;
			case '-':
						if(speed == 5) break;
						speed += 1;
						break;
			case '+':
						if(speed == 1) break;
						speed -= 1;
						break;

			default:break;
		}
		flag = 0;
	}
}


int main()
{
	
	create_screen();
	create_snake();
	food();

	speed_set();

	signal(SIGALRM, snake_move); 

	control();
	endwin();
	
	
	return 0;
}
