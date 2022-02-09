#include "board.h"

#include "globals.h"
#include "renderer.h"

// Internal variables
static uchar * pCellArr;

// Internal functions
static short ToIndex(const ushort x, const ushort y);

// Calculate the buffer index using x/y coordinates
static short ToIndex(const ushort x, const ushort y)
{
    return (y * BOARD_WIDTH) + x;
}

// Initialise the board resources
void BoardInitialise(void)
{
    // calloc should set all values to 0 (cTypeFree)
    pCellArr = (uchar *)calloc(BOARD_WIDTH * BOARD_HEIGHT, sizeof(uchar));
}

// Free the board resources
void BoardFree(void)
{
    free(pCellArr);
}

// Draw the board
//
// if DEBUG draw the snake cells and grid
void BoardDraw(void)
{
    SDL_Renderer * pRenderer = GetRenderer();
    int windowWidth = 0;
    int windowHeight = 0;
    RendererGetWindowSize(&windowWidth, &windowHeight);

    const int OFFSET_X = (windowWidth / 2) - ((BOARD_WIDTH * CELL_SIZE) / 2);
    const int OFFSET_Y = (windowHeight / 2) - ((BOARD_HEIGHT * CELL_SIZE) / 2);
    const ushort CELL_PADDING = CELL_SIZE / 5;

    // Draw the border around the board
    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_Rect r;
    r.x = OFFSET_X - CELL_PADDING;
    r.y = OFFSET_Y - CELL_PADDING;
    r.w = (BOARD_WIDTH * CELL_SIZE) + (CELL_PADDING * 2);
    r.h = (BOARD_HEIGHT * CELL_SIZE) + (CELL_PADDING * 2);
    SDL_RenderFillRect(pRenderer, &r);

    for(ushort x = 0; x < BOARD_WIDTH; x++)
    {
        for(ushort y = 0; y < BOARD_HEIGHT; y++)
        {
            const Celltype CURRENT = BoardGetCell(x, y);
            if(CURRENT == cTypeWall)
            {
                SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

                // Evil macro function
                // Draw the wall segments for the edges
                // Each wall is 1/5 thickness of the cell
                // This function will calculate the position and
                // orientation of the rectangles
                #define DRAW_CELL(axis, isAxisX, floor, ceiling)\
                do\
                {\
                    if(axis != floor && axis != ceiling) { break; }\
                    \
                    SDL_Rect r;\
                    r.x = x * CELL_SIZE;\
                    r.y = y * CELL_SIZE;\
                    r.w = CELL_SIZE;\
                    r.h = CELL_SIZE;\
                    \
                    if(isAxisX)\
                    {\
                        r.x = (axis == floor) ? axis * CELL_SIZE : (axis * CELL_SIZE) + (CELL_PADDING * 4);\
                        r.w = CELL_PADDING;\
                    }\
                    else\
                    {\
                        r.y = (axis == floor) ? axis * CELL_SIZE : (axis * CELL_SIZE) + (CELL_PADDING * 4);\
                        r.h = CELL_PADDING;\
                    }\
                    r.x += OFFSET_X;\
                    r.y += OFFSET_Y;\
                    SDL_RenderFillRect(pRenderer, &r);\
                }while(FALSE)
                DRAW_CELL(x, TRUE, 0, BOARD_WIDTH - 1);
                DRAW_CELL(y, FALSE, 0, BOARD_HEIGHT - 1);
                #undef DRAW_CELL
                continue;
            }

            if(CURRENT == cTypeFood)
            {
                SDL_SetRenderDrawColor(pRenderer, 155, 155, 155, SDL_ALPHA_OPAQUE);
            }
            // Draw the cells where the snake is supposed to be on
            #ifdef DEBUG
            else if(CURRENT == cTypeSnake)
            {
                SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
            }
            #endif // DEBUG
            else
            {
                SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            }

            SDL_Rect r;
            r.w = CELL_SIZE;
            r.h = CELL_SIZE;
            r.x = OFFSET_X + (x * CELL_SIZE);
            r.y = OFFSET_Y + (y * CELL_SIZE);

            SDL_RenderFillRect(pRenderer, &r);
        }
    }

    // Draw the grid
    #ifdef DEBUG
    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for(unsigned short x = 0; x <= BOARD_WIDTH * CELL_SIZE; x += CELL_SIZE)
    {
        SDL_RenderDrawLine(pRenderer, OFFSET_X + x, OFFSET_Y, OFFSET_X + x, OFFSET_Y + (BOARD_HEIGHT * CELL_SIZE));
    }
    for(unsigned short y = 0; y <= BOARD_HEIGHT * CELL_SIZE; y += CELL_SIZE)
    {
        SDL_RenderDrawLine(pRenderer, OFFSET_X, OFFSET_Y + y, OFFSET_X + (BOARD_WIDTH * CELL_SIZE), OFFSET_Y + y);
    }
    #endif // DEBUG
}

// Determine if the tile in x/y position is a free tile
BOOL BoardIsTileFree(const ushort x, const ushort y)
{
    if(x < BOARD_WIDTH && y < BOARD_HEIGHT)
    {
        const short INDEX = ToIndex(x, y);
        return (pCellArr[INDEX] == (uchar)cTypeFree);
    }
    return FALSE;
}

// Set the cell type in specified x/y position
// If index out of range, ignore
void BoardSetCell(const ushort x, const ushort y, const Celltype cell)
{
    if(x < BOARD_WIDTH && y < BOARD_HEIGHT)
    {
        pCellArr[ToIndex(x, y)] = cell;
    }
}

// Return the cell at specified x/y position
Celltype BoardGetCell(const ushort x, const ushort y)
{
    if(x < BOARD_WIDTH && y < BOARD_HEIGHT)
    {
        return pCellArr[ToIndex(x, y)];
    }
    return cTypeFree;
}

// Check if the board is "complete" (no free cells left)
BOOL BoardIsComplete(void)
{
    ushort count = 0;
    for(int i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++)
    {
        if(pCellArr[i] == (uchar)cTypeFree)
        {
            count++;
        }
    }
    return count == 0;
}

// Find free cells and put their positions into the point array
void BoardGetFreeCells(const ushort snakeLength, Point ** ppArr, ushort * pLength)
{
    if(*ppArr)
    {
        free(ppArr);
    }

    const uint INIT_LENGTH = (BOARD_WIDTH * BOARD_HEIGHT) - snakeLength;
    Point * pPointArr = (Point *)malloc(INIT_LENGTH * sizeof(Point));

    uint i = 0;
    for(ushort x = 0; x < BOARD_WIDTH; x++)
    {
        for(ushort y = 0; y < BOARD_HEIGHT; y++)
        {
            const short INDEX = ToIndex(x, y);
            if(pCellArr[INDEX] == (uchar)cTypeFree)
            {
                pPointArr[i].x = x;
                pPointArr[i].y = y;
                i++;
            }
        }
    }
    *pLength = i;
    *ppArr = pPointArr;
}

// Find random free cell and turn it into a food cell
BOOL BoardGenerateFood(const ushort snakeLength)
{
    Point * pFreePoints = NULL;
    ushort length = 0;
    BoardGetFreeCells(snakeLength, &pFreePoints, &length);

    if(length == 0)
    {
        return FALSE;
    }
    if(length == 1)
    {
        BoardSetCell(pFreePoints[0].x, pFreePoints[0].y, cTypeFood);
    }
    else
    {
        const Point RAND_POINT = pFreePoints[rand() % length];
        BoardSetCell(RAND_POINT.x, RAND_POINT.y, cTypeFood);
    }

    if(pFreePoints)
    {
        free(pFreePoints);
    }
    return TRUE;
}

// Determine the center of the board and set the out argument
void BoardGetMidPoint(Point * pPoint)
{
    if(pPoint)
    {
        pPoint->x = BOARD_WIDTH / 2;
        pPoint->y = BOARD_HEIGHT / 2;
    }
}
