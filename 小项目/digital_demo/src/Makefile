CC = gcc


TARGET = main


SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))


$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lcurses

$(OBJS):%.o:%.c
	$(CC) -c $^ -o $@ -lcurses


clean:
	rm *.o 

