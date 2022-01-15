#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "gameboard.h"
#include "snake.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800
#define CELL_SIZE	  8

char GenerateNewFood(GameBoard * board)
{
    Point * pFreePoints = NULL;
    unsigned short length = 0;
    GameBoardGetFree(board, 0, &pFreePoints, &length);
    if(length == 0)
    {
        return 0;
    }

    if(length == 1)
    {
        GameBoardSetCell(board, pFreePoints[0].x, pFreePoints[0].y, cTypeFood);
    }
    else
    {
        const Point point = pFreePoints[rand() % length];
        GameBoardSetCell(board, point.x, point.y, cTypeFood);
    }

    if(pFreePoints != NULL)
    {
        free(pFreePoints);
    }
    return 1;
}

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

    Point initPoint;
    initPoint.x = 50;
    initPoint.y = 50;
    Snake snake;
    SnakeInit(&snake, 100 * 100, initPoint);

    char running = 1;
    while(running)
    {
        // Clear screen and set renderer colour to white
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(pRenderer);

        char isNewFood = 0;
        if(isNewFood)
        {
            running = GenerateNewFood(&board);
        }

        // Check events to resolve exit and to keep app responsive
        const unsigned char * pKeys = SDL_GetKeyboardState(NULL);
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT || pKeys[SDL_SCANCODE_ESCAPE])
            {
                running = 0;
                break;
            }
            else if(pKeys[SDL_SCANCODE_UP])
            {
                SnakeChangeDirection(&snake, cDirectionUp);
                break;
            }
            else if(pKeys[SDL_SCANCODE_DOWN])
            {
                SnakeChangeDirection(&snake, cDirectionDown);
                break;
            }
            else if(pKeys[SDL_SCANCODE_LEFT])
            {
                SnakeChangeDirection(&snake, cDirectionLeft);
                break;
            }
            else if(pKeys[SDL_SCANCODE_RIGHT])
            {
                SnakeChangeDirection(&snake, cDirectionRight);
                break;
            }
        }

        // Update
        SnakeMove(&snake);

        // Draw
        SnakeDraw(&snake, pRenderer, board.cellsize);
        GameBoardDraw(&board, pRenderer);

        // Render
        SDL_RenderPresent(pRenderer);
        SDL_Delay((1.0 / 1.0) * 1000);
    }
    printf("Game over!\n");

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
