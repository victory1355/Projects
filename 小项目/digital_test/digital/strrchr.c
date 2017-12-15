#include <string.h>
#include <stdio.h>
int main()
{
		char num[] = "hello.mp3";
		char ch  = '.';
		char *p = strrchr(num, ch);
		printf("%s\n", p);

		return 0;
}