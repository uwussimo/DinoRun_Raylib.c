/**
 * @file main.cpp - main file for the game
 * @authors
 * - Mukhammadyusuf Abdurakhimov <mabdurakhimov26@my.whitworth.edu>
 * - Enerelt Enkhtur <eenkhtur26@my.whitworth.edu>
 * - Micah Khatri <mkhatri26@my.whitworth.edu>
 *
 * @brief Dino Game - Final Project for CS172 - Computer Science II
 * @version 0.1
 * @date 2023-05-09
 *
 * @ref
 * - raylib: https://www.raylib.com/
 * - raylib cheatsheet: https://www.raylib.com/cheatsheet/cheatsheet.html
 * - lambda functions: https://www.cplusplus.com/doc/tutorial/functions2/
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include "raylib.h"
#include "../headers/game.h"

using namespace std;

int main()
{

    const int screenWidth = 800;
    const int screenHeight = 450;
    Game game("Dino Game - Final Project", screenWidth, screenHeight, false);
    game.run();
    return 0;
}
