#ifndef TYPES_H
#define TYPES_H
#include <stdio.h>

#define BASE_LOCATION -1
#define YX 2
#define RX 28
#define GX 41
#define BX 15
#define HOME_START_YELLOW 0
#define HOME_START_RED 26
#define HOME_START_GREEN 39
#define HOME_START_BLUE 13

#define NUM_PLAYERS 4
#define NUM_PIECES 4
#define BOARD_SIZE 52
#define HOME_PATH_LENGTH 6

typedef enum { BASE, BOARD, HOME_STRAIGHT, HOME } Status;
typedef enum { CLOCKWISE = 1, ANTICLOCKWISE = -1 } Direction;


typedef struct {
    char id[3];  // e.g., "r1", "r2"
    int location;
    int captured;
    int xpass;//x position passing counter
    int isBlocked;  // 1 if the piece is blocked, 0 otherwise
    int isEnergized; // 1 if the piece is energized, 0 otherwise
    int isSick;     // 1 if the piece is sick, 0 otherwise
    int skipRounds; // Number of rounds the piece cannot move
    Status status;
    Direction direction;
} Piece;

typedef struct {
    char color[10];  // e.g., "Red", "Yellow"
    Piece pieces[NUM_PIECES];
    int piecesInBoard;
    int piecesInHome;
    int piecesInBase;
    int homeStart;
} Player;

#endif
