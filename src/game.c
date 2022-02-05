#include "game.h"
#include "renderer.h"
#include "board.h"

// Initialise game board and the snake
void GameInitialise(void)
{
    MenuInitialise(cStateMenu);
    BoardInitialise();

    Point initPoint;
    BoardGetMidPoint(&initPoint);

    SnakeInitialise(initPoint);

    SnakePart * pCurrent = NULL;
    for(pCurrent = SnakeGetHead(); pCurrent; pCurrent = pCurrent->pNext)
    {
        BoardSetCell(pCurrent->point.x, pCurrent->point.y, cTypeSnake);
    }
    BoardGenerateFood();

    state       = cStateMenu;
    snakeLength = SnakeGetLength();
    isVictory   = FALSE;
    timer       = 0.0;
}

// Free gameboard and snake resources
void GameFree(void)
{
    BoardFree();
    SnakeFree();
    MenuFree();
}

// Run the game until exit event is triggered
void GameRun(void)
{
    double currentTime = SDL_GetTicks();
    SDL_KeyCode keycode = SDLK_UNKNOWN;

    while(state != cStateExit)
    {
        RendererClear();

        double newTime = SDL_GetTicks();
        timer += newTime - currentTime;
        currentTime = newTime;

        GetInput(&keycode);
#ifdef DEBUG
        if(keycode == SDLK_ESCAPE)
        {
            state = cStateExit;
            return;
        }
#endif // DEBUG
        switch(state)
        {
            case cStateMenu:
                case cStatePause:
                GameStateMenu(keycode);
                break;

            case cStatePlay:
                GameStatePlay(keycode);
                break;

            case cStateGameOver:
                GameStateGameOver(keycode);
                break;

            case cStateExit:
                return;
        }

        if(state == cStateExit)
        {
            return;
        }
        if(state == cStateMenu || state == cStatePause)
        {
            MenuDraw();
        }
        if(state != cStateMenu)
        {
            BoardDraw();
            SnakeDraw();
        }

        RendererDraw();
        SDL_Delay((1.0 / cRefresh) * 1000);
    }
}

// Get the first valid keyboard or exit input
// If exit event, set the internal state to 'exit'
// If a valid keyboard input (keydown), set
//   the pKeyCode and return
static void GetInput(SDL_KeyCode * pKeyCode)
{
    *pKeyCode = SDLK_UNKNOWN;

    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
            state = cStateExit;
            return;
        }
        else if(e.type == SDL_KEYDOWN)
        {
            *pKeyCode = e.key.keysym.sym;
            return;
        }
    }
}

// Update the menu object
// If the menu type does not match the game state
// Change the menu type before updating
static void GameStateMenu(const SDL_KeyCode keyCode)
{
    if(MenuGetType() != state)
    {
        MenuSetType(state);
    }
    MenuUpdate(keyCode, &state);
}

// Update the game's logic
// Process the input and change snake's position and
// movement (if needed)
// Snake's logic is only updated according to the speed value
// eg. speed 1 = 1 fps, speed 10 = 10 fps
static void GameStatePlay(const SDL_KeyCode keyCode)
{
    switch(keyCode)
    {
        case SDLK_UP:
            SnakeChangeDirection(cDirectionUp);
            break;

        case SDLK_DOWN:
            SnakeChangeDirection(cDirectionDown);
            break;

        case SDLK_LEFT:
            SnakeChangeDirection(cDirectionLeft);
            break;

        case SDLK_RIGHT:
            SnakeChangeDirection(cDirectionRight);
            break;

        case SDLK_ESCAPE:
            state = cStatePause;
            break;

#ifdef DEBUG
        case SDLK_KP_PLUS:
            SnakeAddBodyPart();
            printf("Snake length: %u\n", SnakeGetLength());
            break;
#endif // DEBUG

        default:
            break;
    }

    if(!SnakeIsActive() || isVictory || timer < (1.0 / SnakeGetSpeed()) * 1000)
    {
        return;
    }

    // If snake has just eaten, the board state and snake might be out of sync
    // causing the food to spawn on the snake's tail
    // Only update the tail if snake has NOT eaten
    if(SnakeGetLength() == snakeLength)
    {
        const Point TAIL_POINT = SnakeGetTail()->point;
        BoardSetCell(TAIL_POINT.x, TAIL_POINT.y, cTypeFree);
    }
    else
    {
        snakeLength = SnakeGetLength();
    }

    SnakeMove();
    const Point HEAD_POINT = SnakeGetHead()->point;
    if(BoardGetCell(HEAD_POINT.x, HEAD_POINT.y) == cTypeFood)
    {
        SnakeAddBodyPart();
        BoardGenerateFood();
    }
    BoardSetCell(HEAD_POINT.x, HEAD_POINT.y, cTypeSnake);
    timer = 0.0;
}

// Handle 'Game Over' state
static void GameStateGameOver(const SDL_KeyCode keyCode)
{

}
