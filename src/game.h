#ifndef GAME_H
#define GAME_H

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

static GameState state;
static ushort    snakeLength;
static BOOL      isVictory;
static double    timer;

void GameInitialise(void);
void GameFree(void);
void GameRun(void);

static void GetInput(SDL_Keycode * pKeycode);

static void GameStateMenu(const SDL_Keycode keycode);
static void GameStatePlay(const SDL_Keycode keycode);
static void GameStateGameOver(const SDL_Keycode keycode);

#endif // !GAME_H
