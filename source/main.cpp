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
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Enes game");
    InitAudioDevice();
    game.InitGame();


    SetTargetFPS(60);

    Music background_music = LoadMusicStream("../assets/audio/background_music.mp3");
    PlayMusicStream(background_music);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        game.UpdateDrawFrame();
        UpdateMusicStream(background_music);
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseAudioDevice(); // Close audio device
    //--------------------------------------------------------------------------------------
    game.UnloadGame(); // Unload loaded data (textures, sounds, models...)

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
