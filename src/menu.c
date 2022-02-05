#include "menu.h"

#include "renderer.h"
#include "globals.h"

#include <SDL2/SDL_ttf.h>

// Main menu definitions
#define MAIN_MENU_ELEMENT_LENGTH 2
#define MAIN_MENU_STRING_LENGTH 5

const char g_MAIN_MENU_ELEMENTS[MAIN_MENU_ELEMENT_LENGTH][MAIN_MENU_STRING_LENGTH] =
{
    "PLAY",
    "EXIT"
};

const GameState g_MAIN_MENU_ELEMENT_STATE[MAIN_MENU_ELEMENT_LENGTH] =
{
    cStatePlay,
    cStateExit
};

// Initialise the menu of specified type
// Add menu elements
void MenuInitialise(const GameState initialType)
{
    currentCount     = 0;
    maxCount         = 5;
    currentSelection = 0;
    menuType         = initialType;

    pElementArr = (MenuElement *)calloc(maxCount, sizeof(MenuElement));

    switch(menuType)
    {
        case cStateMenu:
            InitialiseMainMenu();
            break;

        case cStatePause:
            InitialisePauseMenu();
            break;

        default:
            break;
    }
}

// Free menu resources
void MenuFree(void)
{
    if(pElementArr)
    {
        free(pElementArr);
        pElementArr = NULL;
    }

    maxCount         = 0;
    currentCount     = 0;
    currentSelection = 0;
}

// Update menu based on the keycode
// Return TRUE if selection is made, otherwise return FALSE
// If selection is made (Enter/Return), set the element state variable
BOOL MenuUpdate(const SDL_KeyCode keycode, GameState * pState)
{
    switch(keycode)
    {
        case SDLK_UP:
            currentSelection = ((currentSelection - 1) + currentCount) % currentCount;
            break;

        case SDLK_DOWN:
            currentSelection = (currentSelection + 1) % currentCount;
            break;

        case SDLK_RETURN:
            *pState = pElementArr[currentSelection].state;
            return TRUE;

        default:
            break;
    }
    return FALSE;
}

// New new menu state
// If new state is different, free and re-initialise
// otherwise, no op
void MenuSetType(const GameState newState)
{
    if(menuType != newState)
    {
        MenuFree();
        MenuInitialise(newState);
    }
}

// Return the current menu type
GameState MenuGetType(void)
{
    return menuType;
}

// Draw the menu
void MenuDraw(void)
{
    SDL_Renderer * pRenderer = GetRenderer();
    TTF_Font     * pFont     = GetFont();

    for(uchar i = 0; i < currentCount; i++)
    {
        SDL_Color colour;
        colour.r = 255;
        colour.g = (i == currentSelection) ? 0 : 255;
        colour.b = (i == currentSelection) ? 0 : 255;
        colour.a = SDL_ALPHA_OPAQUE;

        SDL_Surface * pTextSurface = TTF_RenderText_Solid(pFont, pElementArr[i].displayName, colour);
        SDL_Texture * pTextTexture = SDL_CreateTextureFromSurface(pRenderer, pTextSurface);
        SDL_RenderCopy(pRenderer, pTextTexture, NULL, &pElementArr[i].area);
        SDL_FreeSurface(pTextSurface);
        SDL_DestroyTexture(pTextTexture);
    }
}

// Add element to the current menu and increment count
static void MenuAddElement(const MenuElement element)
{
    if(currentCount < maxCount)
    {
        pElementArr[currentCount++] = element;
    }
}

// Initialise main menu elements
static void InitialiseMainMenu(void)
{
    // TODO: midpoint
    const int x = (BOARD_WIDTH / 2) - 100;
	int y = (BOARD_HEIGHT / 2) - 200;

	for(int i = 0; i < MAIN_MENU_ELEMENT_LENGTH; i++, y += 64)
    {
        MenuElement el;
        memset(el.displayName, '\0', sizeof(el.displayName));
        strcpy(el.displayName, g_MAIN_MENU_ELEMENTS + i);

        el.area.x = x;
        el.area.y = y;
        el.area.w = 128;
        el.area.h = 64;
        el.state = g_MAIN_MENU_ELEMENT_STATE[i];
        MenuAddElement(el);
    }
}

// Initialise pause menu elements
static void InitialisePauseMenu(void)
{

}
