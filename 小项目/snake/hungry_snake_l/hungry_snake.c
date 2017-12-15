#include <stdio.h>
#include <curses.h>			//curses图形库
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define HEAD_CH "O"
#define BODY_CH "w"

#define WIDTH		20	// 宽度
#define HEIGHT		20	// 高度
#define FIELD_SIZE	(WIDTH * HEIGHT)

#define HEAD	0		// 蛇头位置
#define FOOD	0		// 食物标记
#define ERROR		-1111	// 方向错误码
#define UNDEFINED	((HEIGHT+1)*(WIDTH+1))	// 无效区域标记
#define SNAKE	(UNDEFINED * 2)				// 蛇身体标记
#define S_TIME 30*1000

#define TRUE 1
#define FALSE 0
typedef int BOOL;

// 蛇的结构定义
typedef struct
{
	int size;
	int body [FIELD_SIZE+1];
}Snake;

Snake snake, temsnake;	// 蛇、虚拟蛇

static int board [FIELD_SIZE] = { 0 };
static int temboard [FIELD_SIZE] = { 0 };

enum { LEFT = -1, RIGHT = 1, UP = -WIDTH, DOWN = WIDTH };	// 方向
enum { RED = 0x0C, GREEN = 0x0A, BLUE = 0x09, BLACK = 0x00, WHITE = 0x0F };	// 颜色

static const int mov [4] = { LEFT, RIGHT, UP, DOWN };			// 运动方向数组
static int food;			// 食物位置

unsigned int rudong = 0;
//=================== 循环队列 ===================
// 队列
typedef struct
{
	int front;
	int rear;
	int num [FIELD_SIZE];
}Queue;

Queue queue;

// 进队
void push (Queue* pqueue, int value)
{
	pqueue->num [++pqueue->rear % FIELD_SIZE] = value;
}

// 出队
int pop (Queue* pqueue)
{
	return pqueue->num [++pqueue->front % FIELD_SIZE];
}

// 在 x, y 输出
/*
void gotoXY (short x, short y)
{
	static COORD pos;
	pos.X = ( short ) ( x << 1 );
	pos.Y = y;
	SetConsoleCursorPosition (g_hConsoleOutput, pos);
}*/

// 初始化
void init ()
{
	initscr();								//curses初始化
	curs_set(0);							//隐藏光标
	clear();								//清屏
	refresh();								//屏幕刷新
	
	// 设置随机种子
	srand (( unsigned ) time (NULL));
	memset (&snake, 0, sizeof( Snake ));
	memset (&temsnake, 0, sizeof( Snake ));

	snake.size = 1;
	snake.body [HEAD] = 3 * WIDTH + 4;

	food = 3 * WIDTH + 7;
}

// 检查一个cell有没有被蛇身覆盖
BOOL isCellFree (int idx, Snake* psnake)
{
	int i;
	for ( i = 0; i < psnake->size; ++i ) {
		if ( idx == psnake->body [i] ) return FALSE;
	}
	return TRUE;
}

void newFood ()
{
	BOOL free = FALSE;
	int w, h = 0;
	while ( !free ) {
		w = rand () % ( WIDTH - 2 ) + 1;
		h = rand () % ( HEIGHT - 2 ) + 1;
		food = h * WIDTH + w;
		free = isCellFree (food, &snake);
	}
}


// 检查某个位置idx是否可向move方向运动
BOOL isMovePossible (int idx, int move)
{
	BOOL flag = FALSE;
	switch ( move ) {
	case LEFT:
		if ( idx % WIDTH > 1 ) flag = TRUE; break;
	case RIGHT:
		if ( idx % WIDTH < WIDTH - 2 ) flag = TRUE; break;
	case UP:
		if ( idx > WIDTH * 2 - 1 ) flag = TRUE; break;
	case DOWN:
		if ( idx < FIELD_SIZE - 2 * WIDTH ) flag = TRUE; break;
	}
	return flag;
}

// 重置board
void boardReset (Snake* psnake, int* pboard)
{
	int i;
	for ( i = 0; i < FIELD_SIZE; ++i ) {
		if ( i == food )
			pboard [i] = FOOD;
		else if ( isCellFree (i, psnake) )
			pboard [i] = UNDEFINED;
		else
			pboard [i] = SNAKE;
	}
}

// 广度优先搜索遍历整个board，计算出board中每个非SNAKE元素到达食物的路径长度
BOOL boardRefresh (int pfood, Snake* psnake, int* pboard)
{
	queue.front = queue.rear = 0;
	push (&queue, pfood);
	int inqueue [FIELD_SIZE] = { 0 };
	BOOL found = FALSE;
	// while循环结束后，除了蛇的身体，其它每个方格中的数字代码从它到食物的路径长度
	while ( queue.front != queue.rear ) {
		int idx = pop (&queue);
		if ( inqueue [idx] == 1 ) continue;
		inqueue [idx] = 1;
		int i;
		for ( i = 0; i < 4; ++i ) {
			int nextMove = idx + mov [i];
			if ( !isMovePossible (idx, mov [i]) ) continue;

			if ( nextMove == psnake->body [HEAD] ) {
				found = TRUE;
			}
			if ( pboard [nextMove] >= SNAKE ) continue;

			if ( pboard [nextMove] > pboard [idx] + 1 ) {
				pboard [nextMove] = pboard [idx] + 1;
			}
			if ( inqueue [nextMove] == 0 ) {
				push (&queue, nextMove);
			}
		}
	}
	return found;
}

// 从蛇头开始，根据board中元素值，从蛇头周围4个领域点中选择最短路径
int chooseShortestSafeMove (Snake* psnake, int* pboard)
{
	int bestMove = ERROR;
	int min = SNAKE, i;
	for ( i = 0; i < 4; ++i ) {
		int next = pboard [psnake->body [HEAD] + mov [i]];
		if ( isMovePossible (psnake->body [HEAD], mov [i]) && next < min ) {
			min = next;
			bestMove = mov [i];
		}
	}
	return bestMove;
}

// 从蛇头开始，根据board中元素值，从蛇头周围4个领域点中选择最远路径
int chooseLongestSafeMove (Snake* psnake, int* pboard)
{
	int bestMove = ERROR;
	int max = -1, i;
	for ( i = 0; i < 4; ++i ) {
		int next = pboard [psnake->body [HEAD] + mov [i]];
		if ( isMovePossible (psnake->body [HEAD], mov [i]) && next < UNDEFINED && next > max ) {
			max = next;
			bestMove = mov [i];
		}
	}
	return bestMove;
}

// 真正的蛇在这个函数中，朝 pbestMove 走1步
void makeMove (int pbestMove)
{
	memmove (snake.body + 1, snake.body, sizeof( int ) * ( FIELD_SIZE ));
	snake.body [HEAD] += pbestMove;

	int first = snake.body [HEAD];
	int second = snake.body [HEAD + 1];
	int x = 0, y = 0;
	// 绘制蛇头
	
	mvprintw(first / WIDTH, ( first % WIDTH )*2,  HEAD_CH);

	if ( snake.size > 1 ) {
		mvprintw(second / WIDTH, ( second % WIDTH )*2,  BODY_CH);	
	}

	if ( snake.body [HEAD] == food ) {
		board [snake.body [HEAD]] = SNAKE;
		snake.size++;
		if ( snake.size < FIELD_SIZE ) {
			newFood ();
		}
	}
	else {
		// 擦除蛇尾
		board [snake.body [HEAD]] = SNAKE;
		board [snake.body [snake.size]] = UNDEFINED;

		//first = snake.body [snake.size];
		//second = snake.body [snake.size - 1];

		//x = ( second % WIDTH + first % WIDTH );
		//y = ( second / WIDTH + first / WIDTH );

		//mvprintw(first / WIDTH * 2 , ( first % WIDTH ) * 2 , " ");	
		//mvprintw(y , x , " ");	
		mvprintw(snake.body [snake.size-1]/WIDTH , snake.body [snake.size-1]%WIDTH*2 , " ");
	}
}

// 虚拟地运行一次，然后在调用处检查这次运行可否可行, 可行才真实运行。
// 虚拟运行吃到食物后，得到虚拟下蛇在board的位置
void virtualShortestMove ()
{
	temsnake.size = snake.size;
	memcpy (&temsnake, &snake, sizeof( int ) *( FIELD_SIZE + 1 ));
	memcpy (temboard, board, sizeof( int ) * FIELD_SIZE);
	boardReset (&temsnake, temboard);

	BOOL eatedFood = FALSE;
	while ( !eatedFood ) {
		boardRefresh (food, &temsnake, temboard);
		int move = chooseShortestSafeMove (&temsnake, temboard);
		memmove (temsnake.body + 1, temsnake.body, sizeof( int ) * ( FIELD_SIZE ));
		// 在蛇头前加入一个新的位置
		temsnake.body [HEAD] += move;

		// 如果新加入的蛇头的位置正好是食物的位置
		if ( temsnake.body [HEAD] == food ) {
			temsnake.size++;
			boardReset (&temsnake, temboard);
			temboard [food] = SNAKE;
			eatedFood = TRUE;
		}
		// 如果蛇头不是食物的位置，则新加入的位置为蛇头，最后一个变为空格
		else {
			temboard [temsnake.body [HEAD]] = SNAKE;
			temboard [temsnake.body [HEAD]] = UNDEFINED;
		}
	}
}

// 检查是否可以追着蛇尾运动，即蛇头和蛇尾间是有路径的
BOOL isTailInside ()
{
	temboard [temsnake.body [temsnake.size - 1]] = FOOD;
	temboard [food] = SNAKE;

	BOOL resule = boardRefresh (temsnake.body [temsnake.size - 1], &temsnake, temboard);
	int i;
	for ( i = 0; i < 4; i++ ) {
		if ( isMovePossible (temsnake.body [HEAD], mov [i]) &&
			temsnake.body [HEAD] + mov [i] == temsnake.body [temsnake.size - 1] &&
			temsnake.size > 3 ) {

			resule = FALSE;
		}
	}
	return resule;
}

// 在各种方案都不行时，随便找一个可行的方向来走(1步)
int anyPossibleMove ()
{
	int bestMove = ERROR;
	boardReset (&snake, board);
	boardRefresh (food, &snake, board);
	int min = SNAKE, i;

	for ( i = 0; i < 4; i++ ) {
		if ( isMovePossible (snake.body [HEAD], mov [i]) && board [snake.body [HEAD]] < min ) {
			min = board [snake.body [HEAD] + mov [i]];
			bestMove = mov [i];
		}
	}
	return bestMove;
}

// 让蛇头朝着蛇尾运行一步, 不管蛇身阻挡，朝蛇尾方向运行
int followTail ()
{
	temsnake.size = snake.size;
	memcpy (&temsnake, &snake, sizeof( int ) *( FIELD_SIZE + 1 ));
	boardReset (&temsnake, temboard);

	temboard [temsnake.body [temsnake.size - 1]] = FOOD;		// 让蛇尾成为食物
	temboard [food] = SNAKE;	// 让食物的地方变成蛇身
	boardRefresh (temsnake.body [temsnake.size - 1], &temsnake, temboard);
	temboard [temsnake.body [temsnake.size - 1]] = SNAKE;		// 还原蛇尾

	return chooseLongestSafeMove (&temsnake, temboard);
}

// 如果蛇与食物间有路径，则调用本函数
int findSafeWay ()
{
	int safeMove = ERROR;

	virtualShortestMove ();
	if ( isTailInside () ) {
		return chooseShortestSafeMove (&snake, board);
	}
	safeMove = followTail ();
	return safeMove;
}

/*边界打印*/
void ui_border()
{
	int row = 0;
	int col = 0;
	for(row=0, col=0; 	row <= HEIGHT-1; ++row)				mvprintw( row, col*2, "|");	//左边界
	for(row=1, col=WIDTH-1;	row <= HEIGHT-2; ++row)			mvprintw(row, col*2, "|");	//右边界
	for(row=0, col=0;	col <= WIDTH-2; ++col)				mvprintw( row, col*2, "-");	//上边界
	for(row=HEIGHT-1, col=0;	col <= WIDTH-2; ++col)		mvprintw( row, col*2, "-");	//下边界
}

// 运行
void run ()
{
	// 重置矩阵
	boardReset (&snake, board);

	int bestMove = boardRefresh (food, &snake, board) ? findSafeWay () : followTail ();

	if ( bestMove == ERROR ) {
		bestMove = anyPossibleMove ();
	}
	else {
		makeMove (bestMove);
	}
	ui_border();
	mvprintw(food / WIDTH , (food % WIDTH)*2, "*");	
}

// 开始游戏
void startGame ()
{

	while ( 1 ) 
	{
		//clear();
		run ();
		refresh();
		usleep(S_TIME);
	}
}

int main ()
{
	init ();		// 初始化
	startGame ();	// 开始游戏

	return 0;
}