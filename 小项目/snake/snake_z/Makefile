CC=gcc

TARGET=linklist_test

SRCS=$(wildcard *.c)

OBJS=$(patsubst %.c,%.o,$(SRCS))

$(TARGET):$(OBJS)
	
	$(CC) $(OBJS) -o $(TARGET) -lcurses


$(OBJS):%.o:%.c
	
	$(CC) -c $^ -o $@

clean:
	rm *.o $(TARGET)

