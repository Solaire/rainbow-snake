#include "menu.h"

#include "renderer.h"
#include "globals.h"

#include <SDL2/SDL_ttf.h>

#define ARR_SIZE(a) sizeof(a)/sizeof(a[0])

// Shared constants
#define STRING_TABLE_LENGTH 4
#define MAX_STRING_LENGTH 10

// String table
static const char MENU_STRING_TABLE[STRING_TABLE_LENGTH][MAX_STRING_LENGTH] =
{
    "PLAY",
    "EXIT",
    "MAIN MENU",
    "CONTINUE"
};

// Enum of index to string in the string table
typedef enum
{
    cStringPlay,
    cStringExit,
    cStringMainMenu,
    cStringContinue
} StringIndex;

// Main menu strings
static const StringIndex MAIN_MENU_STRINGS[] =
{
    cStringPlay,
    cStringExit
};

// Main menu states
static const GameState MAIN_MENU_STATES[] =
{
    cStatePlay,
    cStateExit
};

// Pause menu strings
static const StringIndex PAUSE_MENU_STRINGS[] =
{
    cStringContinue,
    cStringMainMenu,
    cStringExit
};

// Main menu states
static const GameState PAUSE_MENU_STATES[] =
{
    cStatePlay,
    cStateMenu,
    cStateExit
};

// Game over menu strings
static const StringIndex GAME_OVER_MENU_STRINGS[] =
{
    cStringMainMenu,
    cStringExit
};

// Game over menu states
static const GameState GAME_OVER_MENU_STATES[] =
{
    cStateMenu,
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
    InitialiseMenuElements();
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
    memset(menuTitle, '\0', sizeof(menuTitle));
}

// Update menu based on the keycode
// Return TRUE if selection is made, otherwise return FALSE
// If selection is made (Enter/Return), set the element state variable
BOOL MenuUpdate(const SDL_Keycode keycode, GameState * pState)
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

    if(menuType == cStatePause && keycode == SDLK_ESCAPE)
    {
        *pState = cStatePlay;
        return TRUE;
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
    // If pause or game over, draw a semi-transparent background
    //if(menuType != cStateMenu)
    if(FALSE)
    {
        int windowWidth = 0;
        int windowHeight = 0;
        RendererGetWindowSize(&windowWidth, &windowHeight);

        SDL_Rect r;
        r.x = (windowWidth / 2) - ((BOARD_WIDTH * CELL_SIZE) / 2);
        r.y = (windowHeight / 2) - ((BOARD_HEIGHT * CELL_SIZE) / 2);
        r.w = (BOARD_WIDTH * CELL_SIZE) + 1;
        r.h = (BOARD_HEIGHT * CELL_SIZE) + 1;

        SDL_SetRenderDrawColor(GetRenderer(), 0, 0, 0, SDL_ALPHA_TRANSPARENT);
        SDL_RenderFillRect(GetRenderer(), &r);
    }

    SDL_Color colour;
    colour.r = 255;
    colour.g = 255;
    colour.b = 255;
    colour.a = SDL_ALPHA_OPAQUE;

    int windowWidth = 0;
    int windowHeight = 0;
    RendererGetWindowSize(&windowWidth, &windowHeight);
    DrawText(menuTitle, colour, (windowWidth / 2) - 100, (windowHeight / 2) - 200);

    for(uchar i = 0; i < currentCount; i++)
    {
        colour.g = (i == currentSelection) ? 0 : 255;
        colour.b = (i == currentSelection) ? 0 : 255;
        colour.a = SDL_ALPHA_OPAQUE;
        DrawText(pElementArr[i].displayName, colour, pElementArr[i].area.x, pElementArr[i].area.y);
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

// Initialise menu elements
static void InitialiseMenuElements(void)
{
    StringIndex * pMenuStrings = NULL;
    GameState   * pMenuStates  = NULL;
    int    menuElementCount = 0;

    switch(menuType)
    {
        case cStateMenu:
            pMenuStrings = MAIN_MENU_STRINGS;
            pMenuStates   = MAIN_MENU_STATES;
            menuElementCount = ARR_SIZE(MAIN_MENU_STRINGS);
            strcpy(menuTitle, "Snake\0");
            break;

        case cStatePause:
            pMenuStrings = PAUSE_MENU_STRINGS;
            pMenuStates   = PAUSE_MENU_STATES;
            menuElementCount = ARR_SIZE(PAUSE_MENU_STRINGS);
            strcpy(menuTitle, "Pause\0");
        break;

        case cStateGameOver:
            pMenuStrings = GAME_OVER_MENU_STRINGS;
            pMenuStates   = GAME_OVER_MENU_STATES;
            menuElementCount = ARR_SIZE(GAME_OVER_MENU_STRINGS);
            strcpy(menuTitle, "Game over\0");
        break;

        default:
            return;
    }

    int windowWidth = 0;
    int windowHeight = 0;
    RendererGetWindowSize(&windowWidth, &windowHeight);

    const int X = (windowWidth / 2) - 100;
    int y = (windowHeight / 2) - 100;

	for(int i = 0; i < menuElementCount; i++, y += 100)
    {
        MenuElement el;
        memset(el.displayName, '\0', sizeof(el.displayName));
        strcpy(el.displayName, MENU_STRING_TABLE[(int)pMenuStrings[i]]);

        el.area.x = X;
        el.area.y = y;
        TTF_SizeText(GetFont(), el.displayName, &el.area.w, &el.area.h);
        el.state = pMenuStates[i];
        MenuAddElement(el);
    }
}

// Draw selected text
static void DrawText(char * pText, const SDL_Color colour, const ushort x, const ushort y)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    TTF_SizeText(GetFont(), pText, &r.w, &r.h);

    SDL_Surface * pTextSurface = TTF_RenderText_Solid(GetFont(), pText, colour);
    SDL_Texture * pTextTexture = SDL_CreateTextureFromSurface(GetRenderer(), pTextSurface);
    SDL_RenderCopy(GetRenderer(), pTextTexture, NULL, &r);
    SDL_FreeSurface(pTextSurface);
    SDL_DestroyTexture(pTextTexture);
}
