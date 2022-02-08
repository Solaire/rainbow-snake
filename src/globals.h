#ifndef GLOBALS_H
#define GLOBALS_H

#include "types.h"

// Game area dimensions
extern const ushort BOARD_WIDTH;
extern const ushort BOARD_HEIGHT;
extern const ushort CELL_SIZE;

// Game definitions
extern const double FPS;

// Snake colour/gradient count
extern const ushort SNAKE_COLOUR_COUNT;
extern const ushort SNAKE_GRADIENT_LENGTH;

// Initial snake length
extern const ushort SNAKE_INIT_LENGTH;

// Calculate the size of array a
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

#endif // !GLOBALS_H
