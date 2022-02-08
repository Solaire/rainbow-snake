#include "renderer.h"
#include <stdio.h>

// "Private" variables
static SDL_Window * pWindow = NULL;
static SDL_Renderer * pRenderer = NULL;
static TTF_Font * pFont = NULL;

// Create the game window.
// Initialise the font and renderer
// Return false if initialisation fails
BOOL RendererInitialise(void)
{
// In debug mode, create borderless fullscreen
// Otherwise the fullscreen will fuck with
// window switching into the debugger
// In release, just make a full screen
#ifdef DEBUG
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    const int MAX_WIDTH  = mode.w;
    const int MAX_HEIGHT = mode.h;

    pWindow = SDL_CreateWindow("Rainbow snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MAX_WIDTH, MAX_HEIGHT, SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
#else
    pWindow = SDL_CreateWindow("Rainbow snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif // DEBUG

    if(!pWindow)
    {
        goto InitialiseRenderer_error;
    }
    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!pRenderer)
    {
        goto InitialiseRenderer_error;
    }
    SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
    if(TTF_Init() != 0)
    {
        goto InitialiseRenderer_error;
    }

    //pFont = TTF_OpenFont("/home/kacper/dev/RainbowSnake/font.ttf", 64); // TODO: path and font size
    pFont = TTF_OpenFont("C:\\dev\\RainbowSnake\\font.ttf", 64); // TODO: path and font size
    if(!pFont)
    {
        printf("fail\n");
        goto InitialiseRenderer_error;
    }

    return TRUE;

InitialiseRenderer_error:
    printf("InitialiseRenderer error: %s\n", SDL_GetError());
    return FALSE;
}

// Free all rendering resources
// Destroy game window
void RendererFree(void)
{
    if(pFont)
    {
        TTF_CloseFont(pFont);
    }
    if(pRenderer)
    {
        SDL_DestroyRenderer(pRenderer);
    }
    if(pWindow)
    {
        SDL_DestroyWindow(pWindow);
    }
}

// Clear the window screen
void RendererClear(void)
{
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(pRenderer);
}

// Draw the present renderer
void RendererDraw(void)
{
    SDL_RenderPresent(pRenderer);
}

// Return the window size
void RendererGetWindowSize(int * pW, int * pH)
{
    if(pWindow)
    {
        SDL_GetWindowSize(pWindow, pW, pH);
    }
}

// Return pointer to the renderer
SDL_Renderer * GetRenderer(void)
{
    return pRenderer;
}

// Return the pointer to the font
TTF_Font * GetFont(void)
{
    return pFont;
}
