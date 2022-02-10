#ifndef BOARD_H
#define BOARD_H

#include "types.h"

typedef enum
{
    cTypeFree = 0,
    cTypeWall = 1,
    cTypeSnake = 2,
    cTypeFood = 3,
} Celltype;

void BoardInitialise(void);
void BoardFree(void);
void BoardDraw(void);
BOOL BoardIsTileFree(const ushort x, const ushort y);
void BoardSetCell(const ushort x, const ushort y, const Celltype cell);
Celltype BoardGetCell(const ushort x, const ushort y);
BOOL BoardIsComplete(void);
void BoardGetFreeCells(const ushort snakeLength, uint * pLength);
BOOL BoardGenerateFood(const ushort snakeLength);
void BoardGetMidPoint(Point * pPoint);

#endif // !BOARD_H
