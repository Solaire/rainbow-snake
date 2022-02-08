#include <time.h>

#include "renderer.h"
#include "game.h"

int main(int argc, char * argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    // Seed rng
    time_t t;
    srand((unsigned) time(&t));

    if(!RendererInitialise())
    {
        return -1;
    }
    GameInitialise();

    // GameRun() will execute until the internal state is 'exit'
    GameRun();

    // Free all resources
    GameFree();
    RendererFree();
    SDL_Quit();
    return 0;
}
