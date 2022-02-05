#include "renderer.h"
#include <stdio.h>

// Create the game window.
// Initialise the font and renderer
// Return false if initialisation fails
BOOL RendererInitialise(void)
{
    pWindow = SDL_CreateWindow("Rainbow snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(!pWindow)
    {
        goto InitialiseRenderer_error;
    }
    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!pRenderer)
    {
        goto InitialiseRenderer_error;
    }
    if(TTF_Init() != 0)
    {
        goto InitialiseRenderer_error;
    }
    pFont = TTF_OpenFont("C:\\dev\\RainbowSnake\\bin\\Win32Debug\\font.ttf", 64); // TODO: path
    if(!pFont)
    {
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
