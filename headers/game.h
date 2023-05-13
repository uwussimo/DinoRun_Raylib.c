/**
 * @file game.h
 * @author Mukhammadyusuf Abdurakhimov <mabdurakhimov26@my.whitworth.edu>
 * @brief Game class for main game loop and other game related functions
 * @version 0.1
 * @date 2023-05-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include "raylib.h"

#define MY_PRIMARY_COLOR \
    CLITERAL(Color) { 88, 255, 129, 255 } // Light-Green

using namespace std;

class Game
{
private:
    string title;
    int width;
    int height;
    Music background_music;
    bool fullscreen = false;

public:
    Game(string title, int width, int height, bool fullscreen = false);
    ~Game();
    void init();
    void update();
    void render();
    void run();
    void loadingScreen();
};

#endif // GAME_H