#ifndef SNAKE_H
#define SNAKE_H

#include "gameboard.h"

enum Direction
{
    cDirectionNone = -1,
    cDirectionUp   = 0,
    cDirectionDown = 1,
    cDirectionLeft = 3,
    cDirectionRight = 4
};

typedef enum Direction Direction;

struct RGB
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};
typedef struct RGB RGB;

struct SnakePart
{
    Point point;
    struct SnakePart * pPrev;
    struct SnakePart * pNext;
};
typedef struct SnakePart SnakePart;

struct Snake
{
    unsigned short finalLength;
    unsigned short currentLength;
    Direction dir;
    RGB * colours;

    SnakePart * pHead;
    SnakePart * pTail;
};

typedef struct Snake Snake;

void SnakeInit(Snake * pSnake, const unsigned short finalLength, const Point initPoint);
void SnakeFree(Snake * pSnake);
void SnakeMove(Snake * pSnake);
void SnakeChangeDirection(Snake * pSnake, const Direction newDirection);
void SnakeAddBodyPart(Snake * pSnake, const Point point);
void SnakeDraw(Snake * pSnake, SDL_Renderer * pRenderer, const unsigned short cellsize);

#endif // !SNAKE_H
