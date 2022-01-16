#ifndef SNAKE_H
#define SNAKE_H

#include "gameboard.h"

// Direction enum for the snake
enum Direction
{
    cDirectionNone = -1,
    cDirectionUp   = 0,
    cDirectionDown = 1,
    cDirectionLeft = 3,
    cDirectionRight = 4
};
typedef enum Direction Direction;

// Structure representing RGB colour
struct RGB
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};
typedef struct RGB RGB;

// Structure representing a single snake cell
// This structure is a doubly-linked list
struct SnakePart
{
    Point point;
    struct SnakePart * prev;
    struct SnakePart * next;
};
typedef struct SnakePart SnakePart;

// Lower-level functions for adding/removing snake cells
static void SnakePartPushHead(SnakePart ** head, SnakePart ** tail, const Point point);
static void SnakePartPushTail(SnakePart ** head, SnakePart ** tail, const Point point);
static SnakePart * SnakePartPopHead(SnakePart ** head, SnakePart ** tail);
static SnakePart * SnakePartPopTail(SnakePart ** head, SnakePart ** tail);
static unsigned short SnakePartLength(SnakePart * head);
static void SnakePartFree(SnakePart * head);
static char SnakePartIsEmpty(SnakePart * head);

// Main snake structure
// Contains the length, direction
// colour array and the pointers to head and tail cells
struct Snake
{
    unsigned short length;
    Direction dir;
    RGB * colours;
    short speed;

    SnakePart * head;
    SnakePart * tail;
};
typedef struct Snake Snake;

// Higher-level functions for managing the snake structure
void SnakeInit(Snake * pSnake, const Point initPoint);
void SnakeFree(Snake * pSnake);
void SnakeMove(Snake * pSnake);
void SnakeChangeDirection(Snake * pSnake, const Direction newDirection);
void SnakeGetNextPos(Snake * pSnake, short * nx, short * ny);
void SnakeAddBodyPart(Snake * pSnake);
void SnakeRemoveBodyPart(Snake * pSnake);
void SnakeDraw(Snake * pSnake, SDL_Renderer * pRenderer, const unsigned short cellsize);

#endif // !SNAKE_H
