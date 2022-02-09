#ifndef TYPES_H
#define TYPES_H

// Basic bool type
typedef unsigned char BOOL;
#ifndef TRUE
#define TRUE 1
#endif // !TRUE

#ifndef FALSE
#define FALSE 0
#endif // !FALSE

// Shorthand typedefs for unsigned stuff
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

// Shared enums

// Game-state enum
// Also used to determine menu type
typedef enum
{
    cStateMenu      = 0,
    cStatePlay      = 1,
    cStatePause     = 2,
    cStateVictory   = 3,
    cStateDefeat    = 4,
    cStateExit      = 5,
} GameState;

// Shared structures

// Cartesian coordinate
typedef struct
{
    int x;
    int y;
} Point;

#endif // !TYPES_H
