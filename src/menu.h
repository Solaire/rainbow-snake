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

static char menuTitle[25];
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
static void InitialiseMenuElements(void);
static void DrawText(char * pText, const SDL_Color colour, const ushort x, const ushort y);

#endif // !MENU_H
