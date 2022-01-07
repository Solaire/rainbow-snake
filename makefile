OBJS = src/gameboard.h src/gameboard.c src/main.c

CC = gcc

COMPILER_FLAGS = -g -Wall -I /usr/include/

LINKER_FLAGS = -lSDL2

OBJ_NAME = rainbow.a

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
