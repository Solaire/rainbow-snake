#ifndef SNAKE_H
#define SNAKE_H

#include "types.h"

// Direction enum
// Value '2' is skipped to prevent snake from travelling
// in the opposite direction (more in SnakeChangeDirection() function)
typedef enum
{
    cDirectionUp    = 0,
    cDirectionDown  = 1,
    cDirectionLeft  = 3,
    cDirectionRight = 4,
} Direction;

// RGB struct
typedef struct
{
    uchar r;
    uchar g;
    uchar b;
} RGB;

// Snake node as a doubly linked-list
struct SnakePart
{
    Point point;
    struct SnakePart * pPrev;
    struct SnakePart * pNext;
};

typedef struct SnakePart SnakePart;

// Some linked-list management functions
static void SnakePartPushHead(const Point point);
static void SnakePartPushTail(const Point point);
static SnakePart * SnakePartPopHead(void);
static SnakePart * SnakePartPopTail(void);
static unsigned short SnakePartLength(void);
static void SnakePartFree(void);
static BOOL SnakePartIsEmpty(void);

static Direction direction;
static ushort length;
static ushort speed;
static BOOL isActive;

static RGB * pColourArr;
static SnakePart * pHead;
static SnakePart * pTail;

void SnakeInitialise(Point initialPoint);
void SnakeFree(void);
void SnakeMove(void);
void SnakeChangeDirection(const Direction newDirection);
void SnakeGetNextPos(Point * pNewPoint);
void SnakeAddBodyPart(void);
void SnakeDraw(void);
ushort SnakeGetLength(void);
SnakePart * SnakeGetHead(void);
SnakePart * SnakeGetTail(void);
ushort SnakeGetSpeed(void);
BOOL SnakeIsActive(void);
BOOL SnakeInBounds(void);

#endif // !SNAKE_H
