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

// Snake node as a doubly linked-list
struct SnakePart
{
    Point point;
    struct SnakePart * pPrev;
    struct SnakePart * pNext;
};

typedef struct SnakePart SnakePart;

void SnakeInitialise(Point initialPoint, const ushort initialLength);
void SnakeFree(void);
void SnakeMove(void);
void SnakeChangeDirection(const Direction newDirection);
void SnakeAddBodyPart(void);
void SnakeDraw(void);
ushort SnakeGetLength(void);
SnakePart * SnakeGetHead(void);
SnakePart * SnakeGetTail(void);
ushort SnakeGetSpeed(void);
BOOL SnakeIsActive(void);
BOOL SnakeInBounds(void);
void SnakeUpdateDirection(void);
void SnakeGetNextPos(Point * pNewPoint);

#endif // !SNAKE_H
