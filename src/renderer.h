#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "types.h"

static SDL_Window * pWindow = NULL;
//static SDL_Surface * pSurface = NULL;
static SDL_Renderer * pRenderer = NULL;
static TTF_Font * pFont = NULL;

BOOL RendererInitialise(void);
void RendererFree(void);
void RendererClear(void);
void RendererDraw(void);
void RendererGetWindowSize(int * pW, int * pH);

SDL_Renderer * GetRenderer(void);
TTF_Font * GetFont(void);

#endif // !RENDERER_H
