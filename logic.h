#ifndef LOGIC_H
#define LOGIC_H

#include "gba.h"

typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;

    /*
    * TA-TODO: Add any logical elements you need to keep track of in your app.
    *
    * For example, for a Snake game, those could be:
    *
    * Snake snake;
    * Food foods[10];
    * int points;
    *
    */
    int won;
    int lives;
    int frogrow;
    int frogcol;
    int truckcolumn;
    int racecarcolumn;
    int bulldozercolumn;
    int purplecarcolumn;
    int yellowcarcolumn;
} AppState;

/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/


//this is to check if the frog collides with the vehicles
int checkCollision(AppState* state, int objectrow, int objectcol, int length, int width);

//This checks if the frog has entered one of the "holes," signifying a victory
int checkVictory(AppState* state);

//Checks that the frog is within the game boundaries
void checkBounds(AppState* state, AppState *nextState);

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif
