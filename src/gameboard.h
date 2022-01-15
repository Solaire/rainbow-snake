#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "SDL2/SDL.h"

enum Celltype
{
    cTypeFree = 0,
    cTypeWall = 1,
    cTypeSnake = 2,
    cTypeFood = 3,
};

typedef enum Celltype Celltype;

struct Point
{
    unsigned short x;
    unsigned short y;
};

typedef struct Point Point;

struct GameBoard
{
    unsigned short width;
    unsigned short height;
    unsigned short cellsize;
    unsigned char * cells;
};

typedef struct GameBoard GameBoard;

void GameBoardInit(GameBoard * pGameBoard, const unsigned short width, const unsigned short height, const unsigned short cellsize);
void GameBoardFree(GameBoard * pGameBoard);
void GameBoardDraw(GameBoard * pGameBoard, SDL_Renderer * pRenderer);
char GameBoardIsValidTile(GameBoard * pGameBoard, const unsigned short x, const unsigned short y);
void GameBoardSetCell(GameBoard * pGameBoard, const unsigned short x, const unsigned short y, const Celltype value);
char GameBoardIsComplete(GameBoard * pGameBoard);
void GameBoardGetFree(GameBoard * pGameboard, const unsigned short snakeLen, Point ** ppPointArr, unsigned short * pLength);

#endif // !GAME_BOARD_H
