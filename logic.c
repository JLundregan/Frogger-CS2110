#include "logic.h"

void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    appState->gameOver = 0;
    appState->lives = 4;
    appState->won = 0;
    appState->frogrow = 147;
    appState->frogcol = 120;
    appState->truckcolumn = randint(0, 240);
    appState->purplecarcolumn = randint(0, 240);
    appState->bulldozercolumn = randint(0, 240);
    appState->racecarcolumn = randint(0, 240);
    appState->yellowcarcolumn = randint(0, 240);
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);



//returns 1 if the frog collides with a car, and returns 0 if no collision
//object row is the starting row of the car/truck, objectcol is the starting column,
//and length and width correspond to the length and width of the car/truck
int checkCollision(AppState* state, int objectrow, int objectcol, int length, int width){
    
    //Note that 13 is the height of the frog, and 11 is the width
    int row = state->frogrow;
    int col = state->frogcol;
    int rightcol = col + 11 - 1;
    int bottom = row + 13 - 1;
    int objectbottom = objectrow+width-1;
    int objectright = objectcol+length-1;

    //This checks if the frog collides with cars coming from the right
    if( (((bottom >= objectrow) && (bottom <= objectbottom)) || ((row <= objectbottom) && (row >= objectrow)))
        && ((rightcol >= objectcol)&&(rightcol<=objectright)))
    {
        return 1;
    }

    if( (((bottom >= objectrow) && (bottom <= objectbottom)) || ((row <= objectbottom) && (row >= objectrow)))
        && ((col <= objectright) && (col >= objectcol)))
    {
        return 1;
    }


    // //This makes sure the frog can't pass over the obstacle
    if( (((rightcol >= objectcol) && (rightcol <= objectright)) || ((col >= objectcol) && (col <= objectright))) 
        && (row == objectbottom || bottom == objectrow)){
        return 1;
    }


    return 0;
}

//Note: these are the (rough) coordinates of the edges of the "victory" alcoves, where 27 is the bottom row,
//16 is the top, and the following pairs represent the sides: (8, 26), (58, 77), (110, 129), (169,180), (212,233)
int checkVictory(AppState* state){
    int row = state->frogrow;
    int col = state->frogcol;
    int rightcol = col + 11 - 1;

    if(rightcol >= 8 && rightcol <= 27 && row == 16){
        return 1;
    }
    if(rightcol >= 58 && rightcol <= 78 && row == 16){
        return 1;
    }

    if(rightcol >= 110 && rightcol <= 130 && row == 16){
        return 1;
    }
    if(rightcol >= 161 && rightcol <= 181 && row == 16){
        return 1;
    }
    if(rightcol >= 212 && rightcol <=234 && row == 16){
        return 1;
    }

    return 0;
}


//Since the upper bound is strange looking, there are a lot of bounds to check, so this is a separate 
//function to check all of the bounds, including the edges of the screen
void checkBounds(AppState* state, AppState* nextState){
    int row = state->frogrow;
    int col = state->frogcol;
    int rightcol = col + 11 - 1;

    //This makes sure the frog can't leave the screen from the side
    if(state->frogcol > WIDTH-11+1){
        nextState->frogcol = WIDTH-11+1;
    }
    if(state->frogcol < 0){
        nextState->frogcol = 0;
    }

    //This makes sure the frog doesn't leave the screen from the bottom
    if(state->frogrow > HEIGHT-13+1){
        nextState->frogrow = HEIGHT-13+1;
    }

    //The rest of this makes sure the frog can't get past the jagged upper bound

    //This makes sure the frog can't get beyond the protruding green bits
    if(row == 27){
        if(col < 8 || (rightcol > 26 && col < 58) || (rightcol > 77 && col < 110) || (rightcol > 129 && col < 161) 
            || (rightcol > 180 && col < 212) || rightcol > 233 ){
            nextState->frogrow = 27;
        }
    }

    //If the frog is in the alcoves, these checks make sure that it can't go beyond the side walls
    if (row < 27){
        if(col < 8){
            nextState->frogcol = 8;
        }
        if(rightcol > 26 && rightcol < 58){
            nextState->frogcol = 26-11+1;
        }
        if(col < 58 && col > 26){
            nextState->frogcol = 58;
        }
        if(rightcol > 77 && rightcol < 110){
            nextState->frogcol = 77-11+1;
        }
        if(col < 110 && col > 77){
            nextState->frogcol = 110;
        }
        if(rightcol > 129 && rightcol < 161){
            nextState->frogcol = 129-11+1;
        }
        if(col < 161 && col > 129){
            nextState->frogcol = 161;
        }
        if(rightcol > 180 && rightcol < 213){
            nextState->frogcol = 180-11+1;
        }
        if(col < 213 && col > 180){
            nextState->frogcol = 213;
        }
        if(rightcol > 232){
            nextState->frogcol = 232-11+1;
        }
    }


}



// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */

    int row = currentAppState->frogrow;
    int col = currentAppState->frogcol;
    int truckcol = currentAppState->truckcolumn;
    int bullcol = currentAppState->bulldozercolumn;
    int racecol = currentAppState->racecarcolumn;
    int carcol = currentAppState->purplecarcolumn;
    int yellowcol = currentAppState->yellowcarcolumn;

    AppState nextAppState = *currentAppState;

    //This checks to see if the game is over, i.e. player has 0 lives
    if(currentAppState->lives == 0){
        nextAppState.gameOver = 1;
    }


    //this moves all of the cars/obstacles
    if (truckcol < 0){
        
        nextAppState.truckcolumn = 240;
    }
    else{
        nextAppState.truckcolumn = truckcol-1;
    }

    if (bullcol > 240){
        
        nextAppState.bulldozercolumn = 0;
    }
    else{
        nextAppState.bulldozercolumn = bullcol+1;
    }

    if (carcol < 0){
        
        nextAppState.purplecarcolumn = 240;
    }
    else{
        nextAppState.purplecarcolumn = carcol-2;
    }

    if (racecol > 240){
        
        nextAppState.racecarcolumn = 0;
    }
    else{
        nextAppState.racecarcolumn = racecol+3;
    }

    if (yellowcol > 240){
        
        nextAppState.yellowcarcolumn = 0;
    }
    else{
        nextAppState.yellowcarcolumn = yellowcol+2;
    }


    //this moves the frog
    if (KEY_DOWN(BUTTON_UP, keysPressedNow))
    {
        nextAppState.frogrow = row - 1;
    }

    if (KEY_DOWN(BUTTON_DOWN, keysPressedNow))
    {
        nextAppState.frogrow = row + 1;
    }

    if (KEY_DOWN(BUTTON_LEFT, keysPressedNow))
    {
        nextAppState.frogcol = col - 1;
    }

    if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow))
    {
        nextAppState.frogcol = col + 1;
    }


    //Checks to see if the user is exiting by pressing "SELECT"
    if(KEY_DOWN(BUTTON_SELECT, keysPressedNow)){
        nextAppState.gameOver = 1;
    }


    //Here, we are checking for collisions, decreasing the life counter as necessary, and resetting the frog's position
    //after each life is lost
    //While these numbers seem to have come out of nowhere, the first is the row the car occupies, and the following are the size.
    //For example, the truck drives along row 128, with a size of 38x12
    if(checkCollision(currentAppState, 128, truckcol, 38, 12) || checkCollision(currentAppState, 80, carcol, 14, 12) 
        || checkCollision(currentAppState, 105, bullcol, 20, 14) || checkCollision(currentAppState, 65, racecol, 14, 12) 
        ||checkCollision(currentAppState, 42, yellowcol, 15, 17)){
        nextAppState.lives--;
        nextAppState.frogrow = 147;
        nextAppState.frogcol = 120;
    }

    //Pretty clear, but just makes sure the next move is legal
    checkBounds(currentAppState, &nextAppState);

    //As a final check, tells the main program whether or not the user has won
    if(checkVictory(currentAppState) == 1){
        nextAppState.won = 1;
    }


    UNUSED(keysPressedBefore);
    return nextAppState;
}
