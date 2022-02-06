#include "board.h"

#include "globals.h"
#include "renderer.h"

// Calculate the buffer index using x/y coordinates
static short ToIndex(const ushort x, const ushort y, const ushort width)
{
    return (y * width) + x;
}

// Initialise board size and cell array
void BoardInitialise(void)
{
    width    = BOARD_WIDTH / CELL_SIZE;
    height   = BOARD_HEIGHT / CELL_SIZE;
    cellsize = CELL_SIZE;

    // calloc should set all values to 0 (cTypeFree)
    pCellArr = (uchar *)calloc(width * height, sizeof(uchar));
}

// Set all variables to 0 and free the cell array
void BoardFree(void)
{
    width    = 0;
    height   = 0;
    cellsize = 0;

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

    const int OFFSET_X = (windowWidth / 2) - (BOARD_WIDTH / 2);
    const int OFFSET_Y = (windowHeight / 2) - (BOARD_HEIGHT / 2);

    for(ushort x = 0; x < width; x++)
    {
        for(ushort y = 0; y < height; y++)
        {
            const Celltype CURRENT = BoardGetCell(x, y);
            if(CURRENT == cTypeWall)
            {
                SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                const ushort CELL_PADDING = cellsize / 5;

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
                    r.x = x * cellsize;\
                    r.y = y * cellsize;\
                    r.w = cellsize;\
                    r.h = cellsize;\
                    \
                    if(isAxisX)\
                    {\
                        r.x = (axis == floor) ? axis * cellsize : (axis * cellsize) + (CELL_PADDING * 4);\
                        r.w = CELL_PADDING;\
                    }\
                    else\
                    {\
                        r.y = (axis == floor) ? axis * cellsize : (axis * cellsize) + (CELL_PADDING * 4);\
                        r.h = CELL_PADDING;\
                    }\
                    r.x += OFFSET_X;\
                    r.y += OFFSET_Y;\
                    SDL_RenderFillRect(pRenderer, &r);\
                }while(FALSE)
                DRAW_CELL(x, 1, 0, width - 1);
                DRAW_CELL(y, 0, 0, height - 1);
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
            r.w = cellsize;
            r.h = cellsize;
            r.x = OFFSET_X + (x * cellsize);
            r.y = OFFSET_Y + (y * cellsize);

            SDL_RenderFillRect(pRenderer, &r);
        }
    }

    // Draw the grid
    #ifdef DEBUG
    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for(unsigned short x = 0; x <= width * cellsize; x += cellsize)
    {
        SDL_RenderDrawLine(pRenderer, OFFSET_X + x, OFFSET_Y, OFFSET_X + x, OFFSET_Y + (height * cellsize));
    }
    for(unsigned short y = 0; y <= height * cellsize; y += cellsize)
    {
        SDL_RenderDrawLine(pRenderer, OFFSET_X, OFFSET_Y + y, OFFSET_X + (width * cellsize), OFFSET_Y + y);
    }
    #endif // DEBUG
}

// Determine if the tile in x/y position is a free tile
BOOL BoardIsTileFree(const ushort x, const ushort y)
{
    if(x < width && y < height)
    {
        const short INDEX = ToIndex(x, y, width);
        return (pCellArr[INDEX] == (uchar)cTypeFree);
    }
    return FALSE;
}

// Set the cell type in specified x/y position
// If index out of range, ignore
void BoardSetCell(const ushort x, const ushort y, const Celltype cell)
{
    if(x < width && y < height)
    {
        pCellArr[ToIndex(x, y, width)] = cell;
    }
}

// Return the cell at specified x/y position
Celltype BoardGetCell(const ushort x, const ushort y)
{
    if(x < width && y < height)
    {
        return pCellArr[ToIndex(x, y, width)];
    }
    return cTypeFree;
}

// Check if the board is "complete" (no free cells left)
BOOL BoardIsComplete(void)
{
    ushort count = 0;
    for(int i = 0; i < width * height; i++)
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

    const uint INIT_LENGTH = (width * height) - snakeLength;
    Point * pPointArr = (Point *)malloc(INIT_LENGTH * sizeof(Point));

    uint i = 0;
    for(ushort x = 0; x < width; x++)
    {
        for(ushort y = 0; y < height; y++)
        {
            const short INDEX = ToIndex(x, y, width);
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
BOOL BoardGenerateFood(void)
{
    Point * pFreePoints = NULL;
    ushort length = 0;
    BoardGetFreeCells(0, &pFreePoints, &length);
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
        const Point RAND_POINT = pFreePoints[rand() & length];
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
        pPoint->x = width / 2;
        pPoint->y = height / 2;
    }
}
