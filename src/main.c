#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "gameboard.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800
#define CELL_SIZE	  8

/*
typedef struct Point
{
    unsigned short x;
    unsigned short y;
} Point;

typedef struct RGB
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

typedef struct SnakePart
{
    Point point;
    RGB rgb;
} SnakePart;

typedef struct Snake
{
    unsigned short length = 0;
    Point ** pointMap = NULL;
    SnakePart * snakeParts = NULL;
} Snake;

void SnakeInit(Snake * pSnake, const unsigned short length)
{
    pSnake->length = length;
    pSnake->pointMap = (Point **)malloc(length * sizeof(Point *));
    pSnake->snakeParts = (SnakePart *)malloc(length * sizeof(SnakePart));
}
*/

int main(int argc, char * argv[])
{
	SDL_Window * pWindow = NULL;
	SDL_Surface * pSurface = NULL;
	SDL_Renderer * pRenderer = NULL;
	
	// SDL setup
    if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN, &pWindow, &pRenderer) != 0)
    {
        return -1;
    }
    pSurface = SDL_GetWindowSurface(pWindow);
    if (!pSurface)
    {
        return -2;
    }
	
	// Seed the rng
	time_t t;
	srand((unsigned) time(&t));
	
    // Create the game board
    GameBoard board;
    GameBoardInit(&board, 100, 100, CELL_SIZE);

    char running = 1;
    while(running)
    {
        // Clear screen and set renderer colour to white
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(pRenderer);

        // Check events to resolve exit and to keep app responsive
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        unsigned short x = rand() % 100;
        unsigned short y = rand() % 100;
        unsigned short retryCount = 0;
        while(!GameBoardIsValidTile(&board, x, y))
        {
            if(GameBoardIsComplete(&board))
            {
                printf("Game Over!\n");
                running = 0;
                break;
            }
            retryCount++;
            x = rand() % 100;
            y = rand() % 100;
        }
        printf("%d\n", retryCount);

        GameBoardSetCell(&board, x, y, cTypeFood);
        GameBoardDraw(&board, pRenderer);

        // Render
        SDL_RenderPresent(pRenderer);
        SDL_Delay((1.0 / 120.0) * 1000);
    }

    // Cleanup
	GameBoardFree(&board);

    // SDL2 Cleanup
    if(pSurface)
    {
        SDL_FreeSurface(pSurface);
    }
    if(pRenderer)
    {
        SDL_DestroyRenderer(pRenderer);
    }
    if(pWindow)
    {
        SDL_DestroyWindow(pWindow);
    }
    SDL_Quit();

	return 0;
}