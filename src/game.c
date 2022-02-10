#include "game.h"
#include "renderer.h"
#include "board.h"
#include "globals.h"

#include "types.h"
#include "snake.h"
#include "menu.h"

#include <stdio.h>
#include <SDL2/SDL_keyboard.h>

// Enum representing snake's speed range
// and the default game update logic rate
typedef enum
{
    cSpeedMin = 1,
    cSpeedMax = 12,
    cRefresh  = 60
} GameTicks;

// Inspired by the old Nokia snake
// In order to be slightly less annoying, the game will
// essentially pause itself for specified number of frames
// if the player is about to lose
// This "warning" time is quite significant on lower speeds (+ 1 second)
// but on higher speeds it might be pretty insignificant
static const uchar WARNING_FRAMES_ALLOWANCE = 1;

// Internal variables
static GameState state;
static ushort    snakeLength;
static double    timer;
static ushort    score;
static uchar     warningFrames;

// Internal functions
static SDL_Keycode GetInput(void);
static void HandleStateMenu(const SDL_Keycode keycode);
static void HandleStatePlay(const SDL_Keycode keycode);
static void HandleStateGameOver(const SDL_Keycode keycode);
static void Reset(void);
static void DrawScore(void);
static void DrawGameOver(void);

// Public functions

// "Constructor"
// Initialise all data
// Reset game to base state
void GameInitialise(void)
{
    MenuInitialise(cStateMenu);
    state       = cStateMenu;
    Reset();
}

// "Destructor"
// Free all resources
void GameFree(void)
{
    BoardFree();
    SnakeFree();
    MenuFree();
}

// Main game loop
// Process input and handle different game states
// Exit function when state is 'exit'
void GameRun(void)
{
    double currentTime = SDL_GetTicks();

    while(state != cStateExit)
    {
        RendererClear();

        double newTime = SDL_GetTicks();
        timer += newTime - currentTime;
        currentTime = newTime;

        // Handle the current state and change state if needed
        const SDL_Keycode KEY_CODE = GetInput();
        if(state == cStateMenu || state == cStatePause)
        {
            HandleStateMenu(KEY_CODE);
        }
        else if(state == cStatePlay)
        {
            HandleStatePlay(KEY_CODE);
        }
        else if(state == cStateVictory || state == cStateDefeat)
        {
            HandleStateGameOver(KEY_CODE);
        }

        // Other states can invoke exit
        if(state == cStateExit)
        {
            return;
        }

        // Render the relevant game elements
        if(state != cStateMenu)
        {
            DrawScore();
            BoardDraw();
            SnakeDraw();
            if(state == cStatePause)
            {
                MenuDraw();
            }
            else if(state == cStateVictory || state == cStateDefeat)
            {
                DrawGameOver();
            }
        }
        else
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
static SDL_Keycode GetInput(void)
{
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
            state = cStateExit;
            return SDLK_UNKNOWN;
        }
        else if(e.type == SDL_KEYDOWN)
        {
            return e.key.keysym.sym;
        }
    }
    return SDLK_UNKNOWN;
}

// Update the menu object
// If the menu type does not match the game state
// Change the menu type before updating
static void HandleStateMenu(const SDL_Keycode keycode)
{
    const GameState OLD_STATE = state;
    if(MenuGetType() != state)
    {
        MenuSetType(state);
    }
    MenuUpdate(keycode, &state);
    if(OLD_STATE == cStateMenu && state == cStatePlay)
    {
        Reset();
    }
}

// Update the game's logic
// Process the input and change snake's position and
// movement (if needed)
// Snake's logic is only updated according to the speed value
// eg. speed 1 = 1 fps, speed 10 = 10 fps
static void HandleStatePlay(const SDL_Keycode keycode)
{
    switch(keycode)
    {
        case SDLK_UP:
            SnakeSetDirection(cDirectionUp);
            break;

        case SDLK_DOWN:
            SnakeSetDirection(cDirectionDown);
            break;

        case SDLK_LEFT:
            SnakeSetDirection(cDirectionLeft);
            break;

        case SDLK_RIGHT:
            SnakeSetDirection(cDirectionRight);
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

    // Snake has its own framerate
    if(!SnakeIsActive() || timer < (1.0 / SnakeGetSpeed()) * 1000)
    {
        return;
    }
    timer = 0.0;

    const BOOL  HAS_DIRECTION_CHANGED = SnakeUpdateDirection();
    const Point NEXT_HEAD_POINT = SnakeGetNextHeadPoint();

    // Check if snake is about to lose, but ignore if next head position is
    // tail, as the tail will move out of the way
    if(!BoardIsCellValid(NEXT_HEAD_POINT) && !PointsAreEqual(NEXT_HEAD_POINT, SnakeGetTail()->point))
    {
        // If we haven't changed the snake's direction and still have
        // warning frame allowance, use that and return
        if(!HAS_DIRECTION_CHANGED && warningFrames++ < WARNING_FRAMES_ALLOWANCE)
        {
            return;
        }

        // Snake lost. Change state and return
        // We're not updating the position to make it look better
        state = cStateDefeat;
        return;
    }
    warningFrames = 0; // Reset warning frames

    // If snake has just eaten, the board state and snake might be out of sync
    // causing the food to spawn on the snake's tail
    // Only update the tail if snake has NOT eaten
    if(SnakeGetLength() == snakeLength)
    {
        BoardSetCell(SnakeGetTail()->point, cTypeFree);
    }
    else
    {
        snakeLength = SnakeGetLength();
    }

    // Update the snake's position and maybe handle food
    SnakeMove();
    const Point HEAD_POINT = SnakeGetHead()->point;
    BOOL newFood = FALSE;

    if(BoardGetCell(HEAD_POINT) == cTypeFood)
    {
        SnakeAddBodyPart();
        newFood = TRUE;
        score += 5;
    }
    BoardSetCell(HEAD_POINT, cTypeSnake);
    if(newFood && !BoardGenerateFood())
    {
        state = cStateVictory;
    }
}

// Handle game over state (either victory or loss)
static void HandleStateGameOver(const SDL_Keycode keycode)
{
    if(keycode == SDLK_RETURN || keycode == SDLK_ESCAPE)
    {
        state = cStateMenu;
    }
}

// Reset board and game data
static void Reset(void)
{
    BoardFree();
    SnakeFree();

    BoardInitialise();

    const Point INITIAL_POINT = { BOARD_WIDTH / 2, BOARD_HEIGHT / 2 };
    SnakeInitialise(INITIAL_POINT, SNAKE_INIT_LENGTH);

    SnakePart * pCurrent = NULL;
    for(pCurrent = SnakeGetHead(); pCurrent; pCurrent = pCurrent->pNext)
    {
        BoardSetCell(pCurrent->point, cTypeSnake);
    }
    BoardGenerateFood();

    snakeLength = SnakeGetLength();
    timer       = 0.0;
    score       = 0;
    warningFrames = 0;
}

// Draw the score component
static void DrawScore(void)
{
    // We want to pad the score to look like this (0005)
    // Determine maximum possible score
    char pad[10];
    sprintf(pad, "%d", BOARD_WIDTH * BOARD_HEIGHT * 5);

    // Construct the score string
    char text[25];
    snprintf(text, 25, "SCORE: %0*d", (int)strlen(pad), score);

    int windowWidth = 0;
    int windowHeight = 0;
    RendererGetWindowSize(&windowWidth, &windowHeight);

    SDL_Color colour;
    colour.r = 255;
    colour.g = 255;
    colour.b = 255;
    RendererDrawText(text, colour, windowWidth / 2, windowHeight / 8, FALSE);
}

// Draw game over components
static void DrawGameOver(void)
{
    SDL_Color colour;
    colour.r = 255;
    colour.g = 255;
    colour.b = 255;

    int windowWidth = 0;
    int windowHeight = 0;
    RendererGetWindowSize(&windowWidth, &windowHeight);

    char * pMainText = (state == cStateVictory) ? "VICTORY" : "DEFEAT";
    RendererDrawText(pMainText, colour, windowWidth / 2, windowHeight / 4, FALSE);

    char * pPromptText = "Press <Enter> or <Escape> to continue";
    RendererDrawText(pPromptText, colour, windowWidth / 2, windowHeight, FALSE);
}
