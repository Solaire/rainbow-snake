#ifndef BOARD_H
#define BOARD_H

#include "types.h"

static short ToIndex(const ushort x, const ushort y, const ushort width);

typedef enum
{
    cTypeFree = 0,
    cTypeWall = 1,
    cTypeSnake = 2,
    cTypeFood = 3,
} Celltype;

static uchar * pCellArr;

void BoardInitialise(void);
void BoardFree(void);
void BoardDraw(void);
BOOL BoardIsTileFree(const ushort x, const ushort y);
void BoardSetCell(const ushort x, const ushort y, const Celltype cell);
Celltype BoardGetCell(const ushort x, const ushort y);
BOOL BoardIsComplete(void);
void BoardGetFreeCells(const ushort snakeLength, Point ** ppArr, ushort * pLength);
BOOL BoardGenerateFood(void);
void BoardGetMidPoint(Point * pPoint);

#endif // !BOARD_H
