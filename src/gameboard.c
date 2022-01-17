#include "gameboard.h"

static short ToIndex(const short x, const short y, const short width)
{
    return (y * width) + x;
}

// Initialise board width, height and cellsize
// Initialise the cell array and set all cells to free
void GameBoardInit(GameBoard * pGameBoard, const unsigned short width, const unsigned short height, const unsigned short cellsize)
{
    pGameBoard->width    = width;
    pGameBoard->height   = height;
    pGameBoard->cellsize = cellsize;

    pGameBoard->cells    = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    for(int i = 0; i < pGameBoard->width * pGameBoard->height; i++)
    {
        pGameBoard->cells[i] = (unsigned char)cTypeFree;
    }
}

// Reset the board values to 0
// Release the memory from the cells buffer
void GameBoardFree(GameBoard * pGameBoard)
{
    pGameBoard->width    = 0;
    pGameBoard->height   = 0;
    pGameBoard->cellsize = 0;
    free(pGameBoard->cells);
}

// Draw the board, with each cell having a different colour
void GameBoardDraw(GameBoard * pGameBoard, SDL_Renderer * pRenderer)
{
    for(int x = 0; x < pGameBoard->width; x++)
    {
        for(int y = 0; y < pGameBoard->height; y++)
        {
            const unsigned short index = ToIndex(x, y, pGameBoard->width);
            switch(pGameBoard->cells[index])
            {
                case (int)cTypeWall:
                    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                    break;
                #ifndef _DEBUG
                #define _DEBUG
                #endif // !DEBUG

                #ifdef DEBUG
                case (int)cTypeSnake:
                    SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
                    break;
                #endif // DEBUG
                case (int)cTypeFood:
                    SDL_SetRenderDrawColor(pRenderer, 155, 155, 155, SDL_ALPHA_OPAQUE);
                    break;
                case (int)cTypeFree:
                default:
                    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                    break;
            }
            SDL_Rect r;
            r.w = pGameBoard->cellsize;
            r.h = pGameBoard->cellsize;
            r.x = x * pGameBoard->cellsize;
            r.y = y * pGameBoard->cellsize;
            SDL_RenderFillRect(pRenderer, &r);
        }
    }
}

// Return 1 if the cell at specified position is free
// Otherwise return 0
char GameBoardIsValidTile(GameBoard * pGameBoard, const short x, const short y)
{
    if(x >= 0 && x < pGameBoard->width && y >= 0 && y < pGameBoard->height)
    {
        const short index = ToIndex(x, y, pGameBoard->width);
        return (pGameBoard->cells[index] == (unsigned char)cTypeFree);
    }
    return 0;
}

// Set the cell at position to specified value
void GameBoardSetCell(GameBoard * pGameBoard, const short x, const short y, const Celltype value)
{
    if(x >= 0 && x < pGameBoard->width && y >= 0 && y < pGameBoard->height)
    {
        const short index = ToIndex(x, y, pGameBoard->width);
        pGameBoard->cells[index] = (unsigned char)value;
    }
}

Celltype GameBoardGetCell(GameBoard * pGameBoard, const short x, const short y)
{
    if(x >= 0 && x < pGameBoard->width && y >= 0 && y < pGameBoard->height)
    {
        const short index = ToIndex(x, y, pGameBoard->width);
        return pGameBoard->cells[index];
    }
}

// Return 1 if the board no longer contains any free cells
// Otherwise return 0
char GameBoardIsComplete(GameBoard * pGameBoard)
{
    unsigned short count = 0;
    for(int i = 0; i < pGameBoard->width * pGameBoard->height; i++)
    {
        if(pGameBoard->cells[i] != (unsigned char)cTypeFree)
        {
            count++;
        }
    }
    return count == (pGameBoard->width * pGameBoard->height);
}

void GameBoardGetFree(GameBoard * pGameBoard, const unsigned short snakeLen, Point ** ppPointArr, unsigned short * pLength)
{
    if(*ppPointArr != NULL)
    {
        free(ppPointArr);
    }

    const unsigned int initLength = (pGameBoard->width * pGameBoard->height) - snakeLen;
    Point * pPoints = (Point *)malloc(initLength * sizeof(Point));

    unsigned int i = 0;
    for(int x = 0; x < pGameBoard->width; x++)
    {
        for(int y = 0; y < pGameBoard->height; y++)
        {
            const short index = ToIndex(x, y, pGameBoard->width);
            if(pGameBoard->cells[index] == (unsigned char)cTypeFree)
            {
                pPoints[i].x = x;
                pPoints[i].y = y;
                i++;
            }
        }
    }
    *pLength = i;
    *ppPointArr = pPoints;
}
