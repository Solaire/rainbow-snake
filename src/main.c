#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "gameboard.h"
#include "snake.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800
#define CELL_SIZE     160
#define FPS 60.0

char GenerateNewFood(GameBoard * board)
{
    Point * pFreePoints = NULL;
    unsigned short length = 0;
    GameBoardGetFree(board, 0, &pFreePoints, &length);
    if(length == 0)
    {
        return 0;
        printf("No free cells available\n");
    }

    if(length == 1)
    {
        GameBoardSetCell(board, pFreePoints[0].x, pFreePoints[0].y, cTypeFood);
        printf("Last food point: (%d, %d)\n", pFreePoints[0].x, pFreePoints[0].y);
    }
    else
    {
        const Point point = pFreePoints[rand() % length];
        GameBoardSetCell(board, point.x, point.y, cTypeFood);
        printf("New food point: (%d, %d)\n", point.x, point.y);
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
    GameBoardInit(&board, WINDOW_WIDTH / CELL_SIZE, WINDOW_HEIGHT / CELL_SIZE, CELL_SIZE);

    Point initPoint;
    initPoint.x = (WINDOW_WIDTH / CELL_SIZE) / 2;
    initPoint.y = (WINDOW_HEIGHT / CELL_SIZE) / 2;
    Snake snake;
    SnakeInit(&snake, initPoint);

    double currentTime = SDL_GetTicks();
    double elapsed = 0.0;

    char running = GenerateNewFood(&board);
    char gameOver = 0;
    while(running)
    {
        // Clear screen and set renderer colour to white
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(pRenderer);

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
            else if(!gameOver)
            {
                if(pKeys[SDL_SCANCODE_UP])
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
                #define DEBUG
                #ifdef DEBUG
                else if(pKeys[SDL_SCANCODE_KP_MINUS])
                {
                    SnakeRemoveBodyPart(&snake);
                    break;
                }
                else if(pKeys[SDL_SCANCODE_KP_PLUS])
                {
                    SnakeAddBodyPart(&snake);
                    break;
                }
                #endif // DEBUG
            }
        }

        if(gameOver)
        {
            goto render;
        }

        char isNewFood = 0;
        double newTime = SDL_GetTicks();
        elapsed += newTime - currentTime;
        currentTime = newTime;

        if(elapsed > (1.0 / snake.speed) * 1000)
        {
            GameBoardSetCell(&board, snake.tail->point.x, snake.tail->point.y, cTypeFree);

            // Update
            SnakeMove(&snake);
            elapsed = 0.0;

            Celltype cell = GameBoardGetCell(&board, snake.head->point.x, snake.head->point.y);
            if(cell == cTypeFood)
            {
                SnakeAddBodyPart(&snake);
                isNewFood = 1;
            }
            else if(cell == cTypeSnake)
            {
                gameOver = 1;
                printf("Game over!\n");
            }

            GameBoardSetCell(&board, snake.head->point.x, snake.head->point.y, cTypeSnake);
        }

        if(isNewFood)
        {
            running = GenerateNewFood(&board);
            isNewFood = 0;
        }

        render:
        // Draw
        GameBoardDraw(&board, pRenderer);
        SnakeDraw(&snake, pRenderer, board.cellsize);

        // Render
        SDL_RenderPresent(pRenderer);
        SDL_Delay((1.0 / FPS) * 1000);
    }
    printf("Finished!\n");

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
