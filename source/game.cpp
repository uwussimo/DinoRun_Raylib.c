#include <iostream>
#include "raylib.h"
#include "../headers/game.h"

using namespace std;

Game::Game()
{
}

void Game::gameLoop(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Dino Run");
    InitAudioDevice();
    InitGame();
    SetTargetFPS(60);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }

    UnloadMusicStream(background_music.music); // Unload music stream buffers from RAM
    // De-Initialization
    CloseAudioDevice(); // Close audio device
    //--------------------------------------------------------------------------------------
    UnloadGame(); // Unload loaded data (textures, sounds, models...)

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}

void Game::InitGame(void)
{
    background_music.music = LoadMusicStream("../assets/audio/background_music.mp3");
    gameOverSound = LoadSound("../assets/audio/game_over.wav");
    winSound = LoadSound("../assets/audio/win.wav");
    jumpSound = LoadSound("../assets/audio/jump_old.wav");
    scoreSound = LoadSound("../assets/audio/score.wav");

    dino.radius = DINO_RADIUS;
    dino.position = {80, screenHeight - 100};
    dino.color = WHITE;

    treeSpeedX = 6;
    for (int i = 0; i < MAX_TREES; i++)
    {
        treesPos[i].x = 800 * i + GetRandomValue(400, 800);
        treesPos[i].y = -GetRandomValue(60, 100);
        cout << "MF:" << treesPos[i].x << endl;
    }

    cout << "---------------------------------" << endl;

    for (int i = 0; i < MAX_TREES * 2; i += 2)
    {
        trees[i + 1].rec.x = treesPos[i / 2].x;
        trees[i + 1].rec.y = 600 + treesPos[i / 2].y - 200;
        trees[i + 1].rec.width = TREES_WIDTH;
        trees[i + 1].rec.height = 200;

        trees[i / 2].active = true;

        cout << trees[i + 1].rec.x << endl
             << trees[i + 1].rec.y << endl
             << endl;
    }

    score = 0;
    gameOver = false;
    superfx = false;
    pause = false;
    win = false;
    background_music.playing = true;
}

// Update game (one frame)
void Game::UpdateGame(void)
{
    UpdateMusicStream(background_music.music);
    if (background_music.playing)
        PlayMusicStream(background_music.music);
    else
        StopMusicStream(background_music.music);

    if (!gameOver)
    {
        if (IsKeyPressed('P'))
        {
            pause = !pause;
            background_music.playing = false;
        }
        if (score == MAX_TREES * 100)
        {
            win = true;
            gameOver = true;
            background_music.playing = false;
            PlaySound(winSound); // Play win sound
        }

        if (!pause)
        {
            for (int i = 0; i < MAX_TREES; i++) // Move trees
                treesPos[i].x -= treeSpeedX;

            for (int i = 0; i < MAX_TREES * 2; i += 2) // Move colliders
            {
                trees[i].rec.x = treesPos[i / 2].x;
                trees[i + 1].rec.x = treesPos[i / 2].x;
            }

            if (IsKeyPressed(KEY_SPACE) && !gameOver && dino.position.y >= 280)
            {
                PlaySound(jumpSound);   // Play jump sound
                dino.position.y -= 200; // Jump
            }

            else if (dino.position.y < screenHeight - 100)
                dino.position.y += 5;

            // Check Collisions
            for (int i = 0; i < MAX_TREES * 2; i++)
            {
                if (CheckCollisionCircleRec(dino.position, dino.radius, trees[i].rec))
                {
                    gameOver = true;
                    pause = false;
                    background_music.playing = false;
                    PlaySound(gameOverSound); // Play game over sound
                }
                else if ((treesPos[i / 2].x < dino.position.x) && trees[i / 2].active && !gameOver)
                {
                    score += 100;
                    PlaySound(scoreSound); // Play score sound
                    if (treeSpeedX >= 14)
                        treeSpeedX += 0.05;
                    else
                        treeSpeedX += 0.7;
                    cout << "TSX: " << treeSpeedX << endl;

                    trees[i / 2].active = false;

                    superfx = true;

                    if (score > hiScore)
                        hiScore = score;
                }
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
        }
    }
}

// Draw game (one frame)
void Game::DrawGame(void)
{
    BeginDrawing();

    ClearBackground(BLUE);

    if (!gameOver)
    {
        DrawCircle(dino.position.x, dino.position.y, dino.radius, dino.color);
        DrawRectangle(0, screenHeight - 100 + dino.radius, screenWidth, 100, GREEN);
        // // Draw trees
        for (int i = 0; i < MAX_TREES; i++)
        {
            DrawRectangle(trees[i * 2].rec.x, trees[i * 2].rec.y, trees[i * 2].rec.width, trees[i * 2].rec.height, BLACK);
            DrawRectangle(trees[i * 2 + 1].rec.x, trees[i * 2 + 1].rec.y, trees[i * 2 + 1].rec.width, trees[i * 2 + 1].rec.height, BLACK);
        }

        // Draw flashing fx (one frame only)
        if (superfx)
        {
            // DrawRectangle(0, 0, screenWidth, screenHeight, BLACK); TODO : Sound effect here
            superfx = false;
        }

        DrawText(TextFormat("%04i", score), 20, 20, 40, WHITE);
        DrawText(TextFormat("HI-SCORE: %04i", hiScore), 20, 70, 20, WHITE);
        DrawFPS(20, 100);

        if (pause)
            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, BLACK);
    }
    else
    {
        win
            ? DrawText("YOU WIN!", GetScreenWidth() / 2 - MeasureText("YOU WIN!", 40) / 2, GetScreenHeight() / 2 - 40, 40, WHITE)
            : DrawText("GAME OVER", GetScreenWidth() / 2 - MeasureText("GAME OVER", 40) / 2, GetScreenHeight() / 2 - 40, 40, WHITE);
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 120, 20, WHITE);
    }

    EndDrawing();
}

// Unload game variables
void Game::UnloadGame(void)
{
}

// Update and Draw (one frame)
void Game::UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}
