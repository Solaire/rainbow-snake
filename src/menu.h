#ifndef MENU_H
#define MENU_H

#include "types.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_keyboard.h>

// Structure defining a single element
typedef struct
{
    char displayName[25];
    SDL_Rect area;
    GameState state;
} MenuElement;

static MenuElement * pElementArr = NULL;
static uchar maxCount;
static uchar currentCount;
static uchar currentSelection;
static GameState menuType;

void MenuInitialise(const GameState initialType);
void MenuFree(void);
BOOL MenuUpdate(const SDL_Keycode keycode, GameState * pState);
void MenuSetType(const GameState newState);
GameState MenuGetType(void);
void MenuDraw(void);

static void MenuAddElement(const MenuElement element);
static void InitialiseMainMenu(void);
static void InitialisePauseMenu(void);

#endif // !MENU_H
