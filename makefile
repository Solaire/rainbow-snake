OBJS = gameboard.h gameboard.c main.c

CC = gcc

COMPILER_FLAGS = -g -wall

LINKER_FLAGS = -lSDL2main -lSDL2

OBJ_NAME = rainbow

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
