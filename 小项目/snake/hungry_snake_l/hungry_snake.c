#include <stdio.h>
#include <curses.h>			//cursesͼ�ο�
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define HEAD_CH "O"
#define BODY_CH "w"

#define WIDTH		20	// ���
#define HEIGHT		20	// �߶�
#define FIELD_SIZE	(WIDTH * HEIGHT)

#define HEAD	0		// ��ͷλ��
#define FOOD	0		// ʳ����
#define ERROR		-1111	// ���������
#define UNDEFINED	((HEIGHT+1)*(WIDTH+1))	// ��Ч������
#define SNAKE	(UNDEFINED * 2)				// ��������
#define S_TIME 30*1000

#define TRUE 1
#define FALSE 0
typedef int BOOL;

// �ߵĽṹ����
typedef struct
{
	int size;
	int body [FIELD_SIZE+1];
}Snake;

Snake snake, temsnake;	// �ߡ�������

static int board [FIELD_SIZE] = { 0 };
static int temboard [FIELD_SIZE] = { 0 };

enum { LEFT = -1, RIGHT = 1, UP = -WIDTH, DOWN = WIDTH };	// ����
enum { RED = 0x0C, GREEN = 0x0A, BLUE = 0x09, BLACK = 0x00, WHITE = 0x0F };	// ��ɫ

static const int mov [4] = { LEFT, RIGHT, UP, DOWN };			// �˶���������
static int food;			// ʳ��λ��

unsigned int rudong = 0;
//=================== ѭ������ ===================
// ����
typedef struct
{
	int front;
	int rear;
	int num [FIELD_SIZE];
}Queue;

Queue queue;

// ����
void push (Queue* pqueue, int value)
{
	pqueue->num [++pqueue->rear % FIELD_SIZE] = value;
}

// ����
int pop (Queue* pqueue)
{
	return pqueue->num [++pqueue->front % FIELD_SIZE];
}

// �� x, y ���
/*
void gotoXY (short x, short y)
{
	static COORD pos;
	pos.X = ( short ) ( x << 1 );
	pos.Y = y;
	SetConsoleCursorPosition (g_hConsoleOutput, pos);
}*/

// ��ʼ��
void init ()
{
	initscr();								//curses��ʼ��
	curs_set(0);							//���ع��
	clear();								//����
	refresh();								//��Ļˢ��
	
	// �����������
	srand (( unsigned ) time (NULL));
	memset (&snake, 0, sizeof( Snake ));
	memset (&temsnake, 0, sizeof( Snake ));

	snake.size = 1;
	snake.body [HEAD] = 3 * WIDTH + 4;

	food = 3 * WIDTH + 7;
}

// ���һ��cell��û�б�������
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


// ���ĳ��λ��idx�Ƿ����move�����˶�
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

// ����board
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

// �������������������board�������board��ÿ����SNAKEԪ�ص���ʳ���·������
BOOL boardRefresh (int pfood, Snake* psnake, int* pboard)
{
	queue.front = queue.rear = 0;
	push (&queue, pfood);
	int inqueue [FIELD_SIZE] = { 0 };
	BOOL found = FALSE;
	// whileѭ�������󣬳����ߵ����壬����ÿ�������е����ִ��������ʳ���·������
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

// ����ͷ��ʼ������board��Ԫ��ֵ������ͷ��Χ4���������ѡ�����·��
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

// ����ͷ��ʼ������board��Ԫ��ֵ������ͷ��Χ4���������ѡ����Զ·��
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

// ������������������У��� pbestMove ��1��
void makeMove (int pbestMove)
{
	memmove (snake.body + 1, snake.body, sizeof( int ) * ( FIELD_SIZE ));
	snake.body [HEAD] += pbestMove;

	int first = snake.body [HEAD];
	int second = snake.body [HEAD + 1];
	int x = 0, y = 0;
	// ������ͷ
	
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
		// ������β
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

// ���������һ�Σ�Ȼ���ڵ��ô����������пɷ����, ���в���ʵ���С�
// �������гԵ�ʳ��󣬵õ�����������board��λ��
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
		// ����ͷǰ����һ���µ�λ��
		temsnake.body [HEAD] += move;

		// ����¼������ͷ��λ��������ʳ���λ��
		if ( temsnake.body [HEAD] == food ) {
			temsnake.size++;
			boardReset (&temsnake, temboard);
			temboard [food] = SNAKE;
			eatedFood = TRUE;
		}
		// �����ͷ����ʳ���λ�ã����¼����λ��Ϊ��ͷ�����һ����Ϊ�ո�
		else {
			temboard [temsnake.body [HEAD]] = SNAKE;
			temboard [temsnake.body [HEAD]] = UNDEFINED;
		}
	}
}

// ����Ƿ����׷����β�˶�������ͷ����β������·����
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

// �ڸ��ַ���������ʱ�������һ�����еķ�������(1��)
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

// ����ͷ������β����һ��, ���������赲������β��������
int followTail ()
{
	temsnake.size = snake.size;
	memcpy (&temsnake, &snake, sizeof( int ) *( FIELD_SIZE + 1 ));
	boardReset (&temsnake, temboard);

	temboard [temsnake.body [temsnake.size - 1]] = FOOD;		// ����β��Ϊʳ��
	temboard [food] = SNAKE;	// ��ʳ��ĵط��������
	boardRefresh (temsnake.body [temsnake.size - 1], &temsnake, temboard);
	temboard [temsnake.body [temsnake.size - 1]] = SNAKE;		// ��ԭ��β

	return chooseLongestSafeMove (&temsnake, temboard);
}

// �������ʳ�����·��������ñ�����
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

/*�߽��ӡ*/
void ui_border()
{
	int row = 0;
	int col = 0;
	for(row=0, col=0; 	row <= HEIGHT-1; ++row)				mvprintw( row, col*2, "|");	//��߽�
	for(row=1, col=WIDTH-1;	row <= HEIGHT-2; ++row)			mvprintw(row, col*2, "|");	//�ұ߽�
	for(row=0, col=0;	col <= WIDTH-2; ++col)				mvprintw( row, col*2, "-");	//�ϱ߽�
	for(row=HEIGHT-1, col=0;	col <= WIDTH-2; ++col)		mvprintw( row, col*2, "-");	//�±߽�
}

// ����
void run ()
{
	// ���þ���
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

// ��ʼ��Ϸ
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
	init ();		// ��ʼ��
	startGame ();	// ��ʼ��Ϸ

	return 0;
}