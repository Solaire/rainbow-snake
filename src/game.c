#include "game.h"
#include "renderer.h"
#include "board.h"
#include "globals.h"

#include "types.h"
#include "snake.h"
#include "menu.h"

#include <SDL2/SDL_keyboard.h>

// Enum representing snake's speed range
// and the default game update logic rate
typedef enum
{
    cSpeedMin = 1,
    cSpeedMax = 12,
    cRefresh  = 60
} GameTicks;

// Internal variables
static GameState state;
static ushort    snakeLength;
static BOOL      isVictory;
static double    timer;

// Internal functions
static void GetInput(SDL_Keycode * pKeycode);
static void GameStateMenu(const SDL_Keycode keycode);
static void GameStatePlay(const SDL_Keycode keycode);
static void GameStateGameOver(const SDL_Keycode keycode);
static void GameReset(void);

// Initialise game board and the snake
void GameInitialise(void)
{
    MenuInitialise(cStateMenu);
    state       = cStateMenu;
    GameReset();
}

// Free gameboard and snake resources
void GameFree(void)
{
    BoardFree();
    SnakeFree();
    MenuFree();
}

// Run the game until exit event is triggered
void GameRun(void)
{
    double currentTime = SDL_GetTicks();
    SDL_Keycode keycode = SDLK_UNKNOWN;

    while(state != cStateExit)
    {
        RendererClear();

        double newTime = SDL_GetTicks();
        timer += newTime - currentTime;
        currentTime = newTime;

        GetInput(&keycode);
        switch(state)
        {
            case cStateMenu:
            case cStatePause:
            case cStateGameOver:
                GameStateMenu(keycode);
                break;

            case cStatePlay:
                GameStatePlay(keycode);
                break;

            case cStateExit:
                return;

            default:
                break;
        }
        if(state == cStateExit)
        {
            return;
        }

        if(isVictory)
        {
            state = cStateGameOver;
        }

        if(state != cStateMenu)
        {
            BoardDraw();
            SnakeDraw();
        }
        if(state == cStateMenu || state == cStatePause || state == cStateGameOver)
        {
            MenuDraw();
        }

        RendererDraw();
        SDL_Delay((1.0 / cRefresh) * 1000);
    }
}

// Get the first valid keyboard or exit input
// If exit event, set the internal state to 'exit'
// If a valid keyboard input (keydown), set
//   the pKeyCode and return
static void GetInput(SDL_Keycode * pKeycode)
{
    *pKeycode = SDLK_UNKNOWN;

    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
            state = cStateExit;
            return;
        }
        else if(e.type == SDL_KEYDOWN)
        {
            *pKeycode = e.key.keysym.sym;
            return;
        }
    }
}

// Update the menu object
// If the menu type does not match the game state
// Change the menu type before updating
static void GameStateMenu(const SDL_Keycode keycode)
{
    GameState oldState = state;
    if(MenuGetType() != state)
    {
        MenuSetType(state);
    }
    MenuUpdate(keycode, &state);
    if(oldState == cStateMenu && state == cStatePlay)
    {
        GameReset();
    }
}

// Update the game's logic
// Process the input and change snake's position and
// movement (if needed)
// Snake's logic is only updated according to the speed value
// eg. speed 1 = 1 fps, speed 10 = 10 fps
static void GameStatePlay(const SDL_Keycode keycode)
{
    switch(keycode)
    {
        case SDLK_UP:
            SnakeChangeDirection(cDirectionUp);
            break;

        case SDLK_DOWN:
            SnakeChangeDirection(cDirectionDown);
            break;

        case SDLK_LEFT:
            SnakeChangeDirection(cDirectionLeft);
            break;

        case SDLK_RIGHT:
            SnakeChangeDirection(cDirectionRight);
            break;

        case SDLK_ESCAPE:
            state = cStatePause;
            return;

#ifdef DEBUG
        case SDLK_KP_PLUS:
            SnakeAddBodyPart();
            break;
#endif // DEBUG

        default:
            break;
    }

    if(!SnakeIsActive() || isVictory || timer < (1.0 / SnakeGetSpeed()) * 1000)
    {
        return;
    }
    timer = 0.0;

    // If snake has just eaten, the board state and snake might be out of sync
    // causing the food to spawn on the snake's tail
    // Only update the tail if snake has NOT eaten
    if(SnakeGetLength() == snakeLength)
    {
        const Point TAIL_POINT = SnakeGetTail()->point;
        BoardSetCell(TAIL_POINT.x, TAIL_POINT.y, cTypeFree);
    }
    else
    {
        snakeLength = SnakeGetLength();
    }

    SnakeMove();
    const Point HEAD_POINT = SnakeGetHead()->point;
    const Celltype CELL = BoardGetCell(HEAD_POINT.x, HEAD_POINT.y);
    BOOL newFood = FALSE;

    if(CELL == cTypeFood)
    {
        SnakeAddBodyPart();
        newFood = TRUE;
    }
    else if(CELL == cTypeSnake || !SnakeInBounds())
    {
        isVictory = FALSE;
        state = cStateGameOver;
    }
    BoardSetCell(HEAD_POINT.x, HEAD_POINT.y, cTypeSnake);
    if(newFood)
    {
        isVictory = !BoardGenerateFood(SnakeGetLength());
    }
}

// Reset board and game data
static void GameReset(void)
{
    BoardFree();
    SnakeFree();

    BoardInitialise();

    Point initPoint;
    BoardGetMidPoint(&initPoint);

    SnakeInitialise(initPoint, SNAKE_INIT_LENGTH);

    SnakePart * pCurrent = NULL;
    for(pCurrent = SnakeGetHead(); pCurrent; pCurrent = pCurrent->pNext)
    {
        BoardSetCell(pCurrent->point.x, pCurrent->point.y, cTypeSnake);
    }
    BoardGenerateFood(SNAKE_INIT_LENGTH);

    snakeLength = SnakeGetLength();
    isVictory   = FALSE;
    timer       = 0.0;
}
