CC=gcc
CFLAGS=-g -Wall
OBJS=commands.o func.o makestruct.o myshell.o
TARGET=myfs_shell

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

commands.o: myfs.h func.c commands.c makestruct.c
func.o: myfs.h func.c makestruct.c
makestruct.o: myfs.h makestruct.c
myshell.o: myfs.h myshell.c func.c

clean:
	rm -f *.o
	rm -f $(TARGET)
