#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include <iostream>
#include <string>

using namespace std;

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define MAX_TREES 100
#define DINO_RADIUS 20
#define TREES_WIDTH 32

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct Dino
{
    Vector2 position;
    int radius;
    Color color;
} Dino;

typedef struct Trees
{
    Rectangle rec;
    Color color;
    bool active;
} Trees;

typedef struct MusicPlayer
{
    Music music;
    bool playing;
} MusicPlayer;

static const int screenWidth = 1200;
static const int screenHeight = 480;

class Game
{
private:
    bool gameOver = false;             // Game over flag
    bool pause = false;                // Pause flag
    int score = 0;                     // Current score
    int hiScore = 0;                   // Current hiscore
    Dino dino = {0};                   // Dino entity
    Trees trees[MAX_TREES * 2] = {0};  // Trees entities
    Vector2 treesPos[MAX_TREES] = {0}; // Trees positions on screen
    float treeSpeedX = 0;              // Trees speed on x axis
    bool superfx = false;              // Activate super effects
    bool win = false;                  // Win flag

    MusicPlayer background_music; // Music playing in the background
    Sound gameOverSound;          // Game over sound

public:
    Game();

    void gameLoop(void);

    void InitGame(void);
    void UpdateGame(void);      // Update game (one frame)
    void DrawGame(void);        // Draw game (one frame)
    void UnloadGame(void);      // Unload game variables
    void UpdateDrawFrame(void); // Update and Draw (one frame)
};

#endif // GAME_H
