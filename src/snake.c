#include "snake.h"
#include "globals.h"
#include "renderer.h"

// Create new SnakePart node with specified point
// Set new head pointer
static void SnakePartPushHead(const Point point)
{
    SnakePart * pNewHead = (SnakePart *)calloc(1, sizeof(SnakePart));
    pNewHead->point = point;

    if(SnakePartIsEmpty())
    {
        pTail = pNewHead;
    }
    else
    {
        pHead->pPrev = pNewHead;
    }
    pNewHead->pNext = pHead;
    pHead = pNewHead;
}

// Create new SnakePart node with specified point
// Set new tail pointer
static void SnakePartPushTail(const Point point)
{
    SnakePart * pNewTail = (SnakePart *)calloc(1, sizeof(SnakePart));
    pNewTail->point = point;

    if(SnakePartIsEmpty())
    {
        pTail = NULL;
    }
    else
    {
        pTail->pNext = pNewTail;
        pNewTail->pPrev = pTail;
    }
    pTail = pNewTail;
}

// Remove the head from the linked-list
// Set the head pointer to current head's next
// Return the pointer to current head
// NOTE: Pointer must be freed
static SnakePart * SnakePartPopHead(void)
{
    SnakePart * pCurrentHead = pHead;
    if(pHead->pNext)
    {
        pHead->pNext->pPrev = NULL;
    }
    else
    {
        pTail = NULL;
    }
    pHead = pHead->pNext;
    return pCurrentHead;
}

// Remove the tail from the linked-list
// Set the tail pointer to current tails' previous
// Return the pointer to current tail
// NOTE: Pointer must be freed
static SnakePart * SnakePartPopTail(void)
{
    SnakePart * pCurrentTail = pTail;
    if(pTail->pPrev)
    {
        pTail->pPrev->pNext = NULL;
    }
    else
    {
        pHead = NULL;
    }
    pTail = pTail->pPrev;
    return pCurrentTail;
}

// Return number of nodes in the linked-list
static unsigned short SnakePartLength(void)
{
    ushort length = 0;
    SnakePart * pCurrent = NULL;
    for(pCurrent = pHead; pCurrent; pCurrent = pCurrent->pNext)
    {
        length++;
    }
    return length;
}

// Free all linked-list nodes
static void SnakePartFree(void)
{
    SnakePart * pCurrent = NULL;
    for(pCurrent = pHead; pCurrent;)
    {
        SnakePart * pTemp = pCurrent;
        pCurrent = pCurrent->pNext;
        free(pTemp);
        pTemp = NULL;
    }
}

// Return true if linked-list is empty
static BOOL SnakePartIsEmpty(void)
{
    return !pHead;
}

// Create snake resources
// Initialise the gradient array
void SnakeInitialise(Point initialPoint)
{
    // Snake resources
    direction   = cDirectionRight;
    length      = 5;
    speed       = 1;
    isActive    = FALSE;

    pColourArr = (RGB *)calloc(SNAKE_COLOUR_COUNT, sizeof(RGB));
    pHead      = NULL;
    pTail      = NULL;

    // Create the colours
    for(ushort i = 0; i < SNAKE_COLOUR_COUNT; i++)
    {
        RGB rgb;
        rgb.r = 0;
        rgb.g = 0;
        rgb.b = 0;

        const short COLOUR_PICKER = (i % SNAKE_COLOUR_COUNT) / SNAKE_GRADIENT_LENGTH;
        const short MOD_25        = (i % SNAKE_GRADIENT_LENGTH);

        switch(COLOUR_PICKER)
        {
            case 0:
                rgb.r = 255;
                rgb.g = 10 * MOD_25;
                rgb.b = 0;
                break;

            case 1:
                rgb.r = 255 - 10 * MOD_25;
                rgb.g = 255;
                rgb.b = 0;
                break;

            case 2:
                rgb.r = 0;
                rgb.g = 255;
                rgb.b = 10 * MOD_25;
                break;

            case 3:
                rgb.r = 0;
                rgb.g = 255 - 10 * MOD_25;
                rgb.b = 255;
                break;

            case 4:
                rgb.r = 10 * MOD_25;
                rgb.g = 0;
                rgb.b = 255;
                break;

            case 5:
                rgb.r = 255;
                rgb.g = 0;
                rgb.b = 255 - 10 * MOD_25;
                break;
        }
        pColourArr[i] = rgb;
    }

    // Add 5 snake parts
    SnakePartPushHead(initialPoint);
    for(ushort i = 0; i < 5; i++)
    {
        initialPoint.x--;
        SnakePartPushTail(initialPoint);
    }
}

// Free all snake resources
void SnakeFree(void)
{
    SnakePartFree();
    if(pColourArr)
    {
        free(pColourArr);
        pColourArr = NULL;
    }
    pHead = NULL;
    pTail = NULL;
}

// Update snake's position based on direction
void SnakeMove(void)
{
    if(!isActive)
    {
        return;
    }

    // No point updating the whole snake.
    // Remove the tail and add a new head node.
    SnakePart * pOldTail = SnakePartPopTail();
    if(pOldTail)
    {
        free(pOldTail);
        pOldTail = NULL;
    }
    Point newHeadPoint;
    SnakeGetNextPos(&newHeadPoint);
    SnakePartPushHead(newHeadPoint);
}

// Change snake's direction.
// Ignore if new direction is opposite of current
void SnakeChangeDirection(const Direction newDirection)
{
    if( abs((char)newDirection - (char)direction) > 1 )
    {
        direction = newDirection;
    }
    if(!isActive)
    {
        isActive = TRUE;
    }
}

// Determine the snake's next head position based on direction
void SnakeGetNextPos(Point * pNewPoint)
{
    if(!pHead)
    {
        return;
    }
    *pNewPoint = pHead->point;
    switch(direction)
    {
        case cDirectionUp:
            pNewPoint->y--;
            break;

        case cDirectionDown:
            pNewPoint->y++;
            break;

        case cDirectionLeft:
            pNewPoint->x--;
            break;

        case cDirectionRight:
            pNewPoint->x++;
            break;

        default:
            break;
    }
}

// Add body part to the snake
// New node's position will be the same as the tail
// This will delay the snake's extension by 1 frame
void SnakeAddBodyPart(void)
{
    SnakePartPushTail(pTail->point);
    length++;
    if(length % 5 == 0)
    {
        speed = fmin(++speed, 5);
    }
}

// Draw the snake
void SnakeDraw(void)
{
    ushort colour = 0;
    SnakePart * pCurrent;
    SDL_Renderer * pRenderer = GetRenderer();

    int windowWidth = 0;
    int windowHeight = 0;
    RendererGetWindowSize(&windowWidth, &windowHeight);

    const int OFFSET_X = (windowWidth / 2) - ((BOARD_WIDTH * CELL_SIZE) / 2);
    const int OFFSET_Y = (windowHeight / 2) - ((BOARD_HEIGHT * CELL_SIZE) / 2);

    for(pCurrent = pHead; pCurrent; pCurrent = pCurrent->pNext)
    {
        // Set the correct rainbow colour
        #define MAKE_RGB(rgb) rgb.r, rgb.g, rgb.b
        SDL_SetRenderDrawColor(pRenderer, MAKE_RGB(pColourArr[colour]), SDL_ALPHA_OPAQUE);
        #undef MAKE_RGB

        colour = (colour + 1) % SNAKE_COLOUR_COUNT;

        // Draw this bit when the snake is longer than 1 cell
        // We have to calculate some rectangles for snake turns and stuff
        if(pCurrent->pNext || pCurrent->pPrev)
        {
            // Evil macro function:
            // Find the difference between points a and b
            // The rectangle should have length 4/5 of the cell centred on the cell
            // If the the difference is positive (i.e. going towards the top-left corner)
            // shift the rectangle by 1/5 cell size towards the origin
            #define DRAW_CELL(a, b)\
                do\
                {\
                    const short PADDING = (CELL_SIZE / 5);\
                    Point diff = a;\
                    diff.x -= b.x;\
                    diff.y -= b.y;\
                    \
                    SDL_Rect r;\
                    r.x = (a.x * CELL_SIZE) + PADDING;\
                    r.y = (a.y * CELL_SIZE) + PADDING;\
                    r.w = PADDING * 4;\
                    r.h = PADDING * 3;\
                    \
                    if(diff.x != 0)\
                    {\
                        r.x = (diff.x == 1) ? r.x - PADDING : r.x;\
                        r.x += OFFSET_X;\
                        r.y += OFFSET_Y;\
                        SDL_RenderFillRect(pRenderer, &r);\
                    }\
                    else if(diff.y != 0)\
                    {\
                        r.w = PADDING * 3;\
                        r.h = PADDING * 4;\
                        r.y = (diff.y == 1) ? r.y - PADDING : r.y;\
                        r.x += OFFSET_X;\
                        r.y += OFFSET_Y;\
                        SDL_RenderFillRect(pRenderer, &r);\
                    }\
                }while(0)

            // For each cell, we draw up to two overlapping rectangles so that
            // we can get either a rectangle with one side = cellsize
            // or an "L" shape where the snake is turning, connecting the next and previous cell
            // This is probably not the most efficient method for drawing since we could
            // draw a single polygon or even a cached shape, but it works so don't touch it
            if(pCurrent->pNext) { DRAW_CELL(pCurrent->point, pCurrent->pNext->point); }
            if(pCurrent->pPrev) { DRAW_CELL(pCurrent->point, pCurrent->pPrev->point); }
            #undef DRAW_CELL
        }
        else // Snake is 1 cell big. Draw a simple square
        {
            SDL_Rect r;
            r.x = OFFSET_X + (pCurrent->point.x * CELL_SIZE) + (CELL_SIZE / 5);
            r.y = OFFSET_Y + (pCurrent->point.y * CELL_SIZE) + (CELL_SIZE / 5);
            r.w = (CELL_SIZE / 5) * 3;
            r.h = (CELL_SIZE / 5) * 3;
            SDL_RenderFillRect(pRenderer, &r);
        }
    }
}

// Return the snake's length
ushort SnakeGetLength(void)
{
    return length;
}

// Return the pointer to the head node
SnakePart * SnakeGetHead(void)
{
    return pHead;
}

// Return the pointer to the tail node
SnakePart * SnakeGetTail(void)
{
    return pTail;
}

// Return snake's speed
ushort SnakeGetSpeed(void)
{
    return speed;
}

// Return snake's active flag
BOOL SnakeIsActive(void)
{
    return isActive;
}

// Check if snake's head is within bounds
BOOL SnakeInBounds(void)
{
    const Point HEAD_POINT = pHead->point;
    return ( (HEAD_POINT.x >= 0 && HEAD_POINT.y >= 0) && (HEAD_POINT.x < BOARD_WIDTH && HEAD_POINT.y < BOARD_HEIGHT) );
}
