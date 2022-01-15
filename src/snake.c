#include "snake.h"

// Initialise snake object
// Reserve memory for all parts and colours up front
void SnakeInit(Snake * pSnake, const unsigned short finalLength, const Point initPoint)
{
    pSnake->finalLength = finalLength;
    pSnake->currentLength = 0;
    pSnake->dir = cDirectionNone;
    pSnake->colours = (RGB *)calloc(finalLength, sizeof(RGB));
    pSnake->pTail = NULL;
    pSnake->pHead = NULL;

    // TEMP
    for(unsigned short i = 0; i < finalLength; i++)
    {
        RGB rgb;
        rgb.r = 255;
        rgb.g = 0;
        rgb.b = 0;

        pSnake->colours[i] = rgb;
    }
    SnakeAddBodyPart(pSnake, initPoint);
}

// Free all snake memory
void SnakeFree(Snake * pSnake)
{
    SnakePart * pNode = pSnake->pTail;
    while(pNode)
    {
        if(pNode->pNext)
        {
            pNode = pNode->pNext;
            free(pNode->pPrev);
            pNode->pPrev = NULL;
        }
        else
        {
            free(pNode);
            pNode = NULL;
        }
    }

    if(pSnake->colours != NULL)
    {
        free(pSnake->colours);
    }
    pSnake->pHead = NULL;
    pSnake->pTail = NULL;
}

// Move the snake in the desired direction
void SnakeMove(Snake * pSnake)
{
    /*
    // We should be able to get away with using a circular
    // buffer to remove the tail and set new head position
    pSnake->parts[pSnake->tail++] = NULL;
    Point newHeadPos = pSnake->parts[pSnake->head++];
    */

    if(pSnake->pTail != pSnake->pHead)
    {
        free(pSnake->pTail);
        pSnake->pTail = NULL;
    }

    Point newHeadPos = pSnake->pHead->point;

    // Determine new snake head based on movement
    switch(pSnake->dir)
    {
        case cDirectionUp:
            newHeadPos.y--;
            break;

        case cDirectionDown:
            newHeadPos.y++;
            break;

        case cDirectionLeft:
            newHeadPos.x--;
            break;

        case cDirectionRight:
            newHeadPos.x++;
            break;

        case cDirectionNone:
        default:
            return;
    }
    SnakePart * pNewHead = (SnakePart *)calloc(1, sizeof(SnakePart));
    pNewHead->point = newHeadPos;
    pNewHead->pPrev = pSnake->pHead;
    pSnake->pHead->pNext = pNewHead;
    pSnake->pHead = pNewHead;

    //pSnake->parts[pSnake->head] = newHeadPos;
}

// Change the snake's direction
void SnakeChangeDirection(Snake * pSnake, const Direction newDirection)
{
    // Only set new direction if not going in opposite direction
    if(abs(newDirection - pSnake->dir) > 1)
    {
        pSnake->dir = newDirection;
    }
}

// Add body part to the snake
void SnakeAddBodyPart(Snake * pSnake, const Point point)
{
    SnakePart * pNewTail = (SnakePart *)calloc(1, sizeof(SnakePart));
    pNewTail->point = point;
    pNewTail->pNext = pSnake->pTail;
    if(pSnake->pTail)
    {
        pSnake->pTail->pPrev = pNewTail;
    }
    else
    {
        pSnake->pHead = pNewTail;
    }
    pSnake->pTail = pNewTail;
    /*
    Point p;
    p.x = pSnake->parts[pSnake->head].x;
    p.y = pSnake->parts[pSnake->head].y;
    pSnake->parts[--pSnake->tail] = p;
    */
}

// Draw the snake
void SnakeDraw(Snake * pSnake, SDL_Renderer * pRenderer, const unsigned short cellsize)
{
    unsigned short iRGB = 0;
    //for(unsigned short i = 0; i < pSnake->currentLength; i++)
    SnakePart * pCell = pSnake->pHead;
    while(pCell)
    {
        SDL_Rect r;
        r.x = pCell->point.x * cellsize;
        r.y = pCell->point.y * cellsize;
        r.w = cellsize;
        r.h = cellsize;

        #define MAKE_RGB(rgb) rgb.r, rgb.g, rgb.b
        SDL_SetRenderDrawColor(pRenderer, MAKE_RGB(pSnake->colours[iRGB++]), SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(pRenderer, &r);
        pCell = pCell->pPrev;
    }
}
