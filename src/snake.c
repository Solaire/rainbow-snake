#include "snake.h"

static SnakePart * SnakePartPushHead(SnakePart * head, const Point point)
{
    SnakePart * newHead = (SnakePart*)malloc(sizeof(SnakePart));
    newHead->point = point;
    newHead->prev = NULL;

    head->prev = newHead;
    newHead->next = head;
    return newHead;
}

static SnakePart * SnakePartPushTail(SnakePart * tail, const Point point)
{
    SnakePart * newTail = (SnakePart *)malloc(sizeof(SnakePart));
    newTail->point = point;
    newTail->next = NULL;

    tail->next = newTail;
    newTail->prev = tail;
    return newTail;
}

static SnakePart * SnakePartPopHead(SnakePart * head)
{
    SnakePart * tmp = head;
    if(head->next != NULL)
    {
        head->next->prev = NULL;
    }
    head = head->next;
    return tmp;
}

static SnakePart * SnakePartPopTail(SnakePart * tail)
{
    SnakePart * tmp = tail;
    if(tail->prev != NULL)
    {
        tail->prev->next = NULL;
    }
    tail = tail->prev;
    return tmp;
}

static unsigned short SnakePartLength(SnakePart * head)
{
    unsigned short length = 0;
    SnakePart * current = NULL;
    for(current = head; current != NULL; current = current->next)
    {
        length++;
    }
    return length;
}

static void SnakePartFree(SnakePart * head)
{
    SnakePart * current = NULL;
    for(current = head; current != NULL;)
    {
        SnakePart * tmp = current;
        current = current->next;
        free(tmp);
        tmp = NULL;
    }
}

// Initialise snake object
// Reserve memory for all parts and colours up front
void SnakeInit(Snake * pSnake, const Point initPoint)
{
    pSnake->length = 0;
    pSnake->dir = cDirectionNone;
    pSnake->colours = (RGB *)calloc(100, sizeof(RGB));
    pSnake->tail = NULL;
    pSnake->head = NULL;

    // TEMP
    for(unsigned short i = 0; i < 100; i++)
    {
        RGB rgb;
        rgb.r = 255;
        rgb.g = 0;
        rgb.b = 0;

        pSnake->colours[i] = rgb;
    }
    SnakePart * init = (SnakePart *)malloc(sizeof(SnakePart));
    init->point = initPoint;
    init->next = NULL;
    init->prev = NULL;
    pSnake->head = init;
    pSnake->tail = init;
}

// Free all snake memory
void SnakeFree(Snake * pSnake)
{
    SnakePartFree(pSnake->head);
    /*
    SnakePart * pNode = pSnake->tail;
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
    */
    if(pSnake->colours != NULL)
    {
        free(pSnake->colours);
    }
    pSnake->head = NULL;
    pSnake->tail = NULL;
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

    /*
    if(pSnake->pTail != pSnake->pHead)
    {
        free(pSnake->pTail);
        pSnake->pTail = NULL;
    }
    */

    SnakePart * tail = SnakePartPopTail(pSnake->tail);
    free(tail);

    Point newHeadPos = pSnake->head->point;

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
    pSnake->head = SnakePartPushHead(pSnake->head, newHeadPos);

    /*
    SnakePart * pNewHead = (SnakePart *)calloc(1, sizeof(SnakePart));
    pNewHead->point = newHeadPos;
    pNewHead->pPrev = pSnake->pHead;
    pSnake->pHead->pNext = pNewHead;
    pSnake->pHead = pNewHead;
    */

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
    SnakePartPushTail(pSnake->tail, point);

    /*
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
    */

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

    //SnakePart * pCell = pSnake->head;
    //while(pCell)
    SnakePart * current = NULL;
    for(current = pSnake->head; current != NULL; current = current->next)
    {
        SDL_Rect r;
        r.x = current->point.x * cellsize;
        r.y = current->point.y * cellsize;
        r.w = cellsize;
        r.h = cellsize;

        #define MAKE_RGB(rgb) rgb.r, rgb.g, rgb.b
        SDL_SetRenderDrawColor(pRenderer, MAKE_RGB(pSnake->colours[iRGB]), SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(pRenderer, &r);
        //pCell = pCell->pPrev;
    }
}
