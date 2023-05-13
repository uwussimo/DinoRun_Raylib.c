/**
 * @file game.cpp
 * @author Mukhammadyusuf Abdurakhimov <mabdurakhimov26@my.whitworth.edu>
 * @brief Game class for main game loop and other game related functions
 * @version 0.1
 * @date 2023-05-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../headers/game.h"
#include "../headers/utils.h"

using namespace std;

Game::Game(string title, int width, int height, bool fullscreen)
{
    this->title = title;
    this->width = width;
    this->height = height;
    this->fullscreen = fullscreen;
}

Game::~Game()
{
    UnloadMusicStream(background_music);
    CloseAudioDevice();
    CloseWindow();
}

void Game::init()
{
    InitWindow(width, height, title.c_str());

    if (fullscreen)
        ToggleFullscreen();

    SetTargetFPS(60);
    InitAudioDevice();
    background_music = LoadMusicStream("../assets/audio/background_music.mp3");

    // hide terminal window
}

void Game::update()
{
    UpdateMusicStream(background_music);
}

void Game::run()
{

    init();
    loadingScreen();

    while (!WindowShouldClose())
    {
        update();
        render();
    }
}

void Game::render()
{
    BeginDrawing(); // begin drawing

    ClearBackground(BLACK);

    DrawCentralizedText("Welcome to Dino Game!", 40, MY_PRIMARY_COLOR, width, height - 100);
    DrawButton("Start Game", 20, MY_PRIMARY_COLOR, 200, 50, width / 2 - 100, height / 2);
    DrawButton(
        "Exit", 20, MY_PRIMARY_COLOR, 200, 50, width / 2 - 100, height / 2 + 64, CloseWindow); // pass the function pointer to the callback

    EndDrawing();
}

void Game::loadingScreen()
{
    IsMusicReady(background_music) ? PlayMusicStream(background_music) : TraceLog(LOG_WARNING, "Music is not ready!");
}
