#include <iostream>
#include "raylib.h"
using namespace std;

#include "../headers/game.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    Game game;
    game.gameLoop(); // Initialize game
    return 0;
}
