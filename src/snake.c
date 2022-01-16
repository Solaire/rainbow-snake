#include "snake.h"

#define MAX( a, b ) ( ( a > b) ? a : b )
#define MIN( a, b ) ( ( a < b) ? a : b )

// Add new snake part to the front of the linked list
static void SnakePartPushHead(SnakePart ** head, SnakePart ** tail, const Point point)
{
    SnakePart * newHead = (SnakePart*)calloc(1, sizeof(SnakePart));
    newHead->point = point;

    if(SnakePartIsEmpty(*head))
    {
        *tail = newHead;
    }
    else
    {
        (*head)->prev = newHead;
    }
    newHead->next = *head;
    *head = newHead;
}

// Add new snake part to the end of the linked list
static void SnakePartPushTail(SnakePart ** head, SnakePart ** tail, const Point point)
{
    SnakePart * newTail = (SnakePart *)calloc(1, sizeof(SnakePart));
    newTail->point = point;

    if(SnakePartIsEmpty(*head))
    {
        *tail = NULL;
    }
    else
    {
        (*tail)->next = newTail;
        newTail->prev = *tail;
    }
    *tail = newTail;
}

// Remove the first entry in the SnakePart linked list
// Return pointer to the old head
// NOTE: The pointer must be freed manually
static SnakePart * SnakePartPopHead(SnakePart ** head, SnakePart ** tail)
{
    SnakePart * ret = *head;
    if((*head)->next == NULL)
    {
        *tail = NULL;
    }
    else
    {
        (*head)->next->prev = NULL;
    }
    *head = (*head)->next;
    return ret;
}

// Remove the last entry in the SnakePart linked list
// Return pointer to the old tail
// NOTE: The pointer must be freed manually
static SnakePart * SnakePartPopTail(SnakePart ** head, SnakePart ** tail)
{
    SnakePart * ret = *tail;
    if((*head)->next == NULL)
    {
        *head = NULL;
    }
    else
    {
        (*tail)->prev->next = NULL;
    }
    *tail = (*tail)->prev;
    return ret;
}

// Iterate the SnakePart linked list, starting at front
// and return the number of nodes
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

// Iterate linked list, starting at front
// and free all nodes
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

// Check if SnakePart linked list is empty
static char SnakePartIsEmpty(SnakePart * head)
{
    return head == NULL;
}

// Initialise snake object
// Reserve memory for all parts and colours up front
void SnakeInit(Snake * pSnake, const Point initPoint)
{
    pSnake->length = 0;
    pSnake->dir = cDirectionNone;
    pSnake->colours = (RGB *)calloc(10000, sizeof(RGB));
    pSnake->length = 1;
    pSnake->speed = 5;

    pSnake->tail = NULL;
    pSnake->head = NULL;

    // TEMP
    for(unsigned short i = 0; i < 10000; i++)
    {
        RGB rgb;
        rgb.r = 0;
        rgb.g = 255;
        rgb.b = 0;

        pSnake->colours[i] = rgb;
    }
    SnakePartPushHead(&pSnake->head, &pSnake->tail, initPoint);
}

// Free the snake object
void SnakeFree(Snake * pSnake)
{
    SnakePartFree(pSnake->head);
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
    // No movement, return early
    if(pSnake->dir == cDirectionNone)
    {
        return;
    }

    // Remove the tail
    // Set the new head position based on movement, and add new head
    SnakePart * oldTail = SnakePartPopTail(&pSnake->head, &pSnake->tail);
    Point newHeadPos;
    if(!SnakePartIsEmpty(pSnake->head))
    {
        newHeadPos = pSnake->head->point;
    }
    else
    {
        newHeadPos = oldTail->point;
    }
    free(oldTail);
    oldTail = NULL;

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
    }

    if(newHeadPos.x < 0)
    {
        newHeadPos.x = 24;
    }
    else if(newHeadPos.x > 24)
    {
        newHeadPos.x = 0;
    }

    if(newHeadPos.y < 0)
    {
        newHeadPos.y = 24;
    }
    else if(newHeadPos.y > 24)
    {
        newHeadPos.y = 0;
    }

    SnakePartPushHead(&pSnake->head, &pSnake->tail, newHeadPos);
}

// Change the snake's direction
void SnakeChangeDirection(Snake * pSnake, const Direction newDirection)
{
    // Only set new direction if not going in opposite direction
    if(pSnake->dir == cDirectionNone || abs((char)newDirection - (char)pSnake->dir) > 1)
    {
        pSnake->dir = newDirection;
    }
}

void SnakeGetNextPos(Snake * pSnake, short * nx, short * ny)
{
    if(SnakePartIsEmpty(pSnake->head))
    {
        *nx = -1;
        *ny = -1;
        return;
    }
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
    }
    *nx = newHeadPos.x;
    *ny = newHeadPos.y;
}

// Add body part to the snake
void SnakeAddBodyPart(Snake * pSnake)
{
    SnakePartPushTail(&pSnake->head, &pSnake->tail, pSnake->tail->point);
    pSnake->length++;
    if(pSnake->length % 5 == 0)
    {
        pSnake->speed = MIN(++pSnake->speed, 30);
    }
}

void SnakeRemoveBodyPart(Snake * pSnake)
{
    if(pSnake->length == 1)
    {
        return;
    }
    SnakePart * tmp = SnakePartPopHead(&pSnake->head, &pSnake->tail);
    free(tmp);
    tmp = NULL;
    pSnake->length--;
    if(pSnake->length % 5 == 4)
    {
        pSnake->speed = MAX(1, --pSnake->speed);
    }
}

// Draw the snake
void SnakeDraw(Snake * pSnake, SDL_Renderer * pRenderer, const unsigned short cellsize)
{
    unsigned short iRGB = 0;
    SnakePart * current = NULL;

    // TEMP
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
        iRGB++;
    }
}
