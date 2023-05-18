#include <iostream>
#include "raylib.h"
#include <string>
#include <vector>
#include "../headers/game.h"
#include "../headers/utils.h"

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
    morning.texture = LoadTexture("../assets/sprites/morning.png");
    night.texture = LoadTexture("../assets/sprites/night.png");
    background_music.music = LoadMusicStream("../assets/audio/background_music.mp3");
    gameOverSound = LoadSound("../assets/audio/game_over.wav");
    emotionalSound = LoadSound("../assets/audio/emotional.wav");
    winSound = LoadSound("../assets/audio/win.wav");
    jumpSound = LoadSound("../assets/audio/jump_old.wav");
    scoreSound = LoadSound("../assets/audio/score.wav");

    dino.width = 72;
    dino.height = 122;
    dino.position = {80, screenHeight / 2};
    dino.color = Color{255, 255, 255, 0};
    dino.texture = LoadTexture("../assets/sprites/dino1.png");

    treeSpeedX = 6;
    for (int i = 0; i < MAX_TREES; i++)
    {
        treesPos[i].x = 800 * i + GetRandomValue(1000, 1200);
        treesPos[i].y = -GetRandomValue(60, 100);
    }

    for (int i = 0; i < MAX_TREES * 2; i += 2)
    {
        trees[i + 1].rec.x = treesPos[i / 2].x;
        trees[i + 1].rec.y = 600 + treesPos[i / 2].y - 200;
        trees[i + 1].rec.width = TREES_WIDTH;
        trees[i + 1].rec.height = 200;

        trees[i / 2].active = true;
        trees[i].texture = LoadTexture("../assets/sprites/tree.png");
        trees[i + 1].color = Color{219, 160, 93, 100};
    }
    // set from stats
    hiScore = stoi(stats[0][1]); // stoi converts string to int
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

            if (IsKeyPressed(KEY_SPACE) && !gameOver && dino.position.y >= 200)
            {
                PlaySound(jumpSound);   // Play jump sound
                dino.position.y -= 250; // Jump
            }

            else if (dino.position.y < screenHeight - 200)
                dino.position.y += 5;

            // Check Collisions
            for (int i = 0; i < MAX_TREES * 2; i++)
            {
                if (CheckCollisionRecs(Rectangle{dino.position.x, dino.position.y, float(dino.width), float(dino.height)}, trees[i].rec))
                {
                    gameOver = true;
                    pause = false;
                    background_music.playing = false;
                    score <= 500
                        ? PlaySound(emotionalSound) // Play emotional sound
                        : PlaySound(gameOverSound); // Play game over sound
                }
                else if ((treesPos[i / 2].x < dino.position.x) && trees[i / 2].active && !gameOver)
                {
                    score += 100;
                    PlaySound(scoreSound); // Play score sound
                    if (treeSpeedX >= 14)
                        treeSpeedX += 0.05;
                    else
                        treeSpeedX += 0.7;

                    trees[i / 2].active = false;

                    superfx = true;

                    if (score > hiScore)
                    {
                        hiScore = score;
                        stats[0][1] = to_string(hiScore); // to_string converts int to string
                    }
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

void Game::restartGame()
{
    InitGame();
    gameOver = false;
}

// Draw game (one frame)
void Game::DrawGame(void)
{
    BeginDrawing();

    bool day = true;
    if (score / 500 % 2 == 0)
        day = !day;

    if (day)
    {
        ClearBackground(Color{91, 137, 255, 255});
        DrawTexture(morning.texture, 0, 0, WHITE);
    }
    else
    {
        ClearBackground(Color{0, 0, 0, 255});
        DrawTexture(night.texture, 0, 0, WHITE);
    }
    if (!gameOver)
    {
        // DrawCircle(dino.position.x, dino.position.y, dino.radius, dino.color);
        DrawRectangle(dino.position.x, dino.position.y, dino.width, dino.height, dino.color);
        DrawTexture(dino.texture, dino.position.x, dino.position.y, WHITE);
        DrawTextureRec(dino.texture, {0, 0, (float)dino.texture.width, (float)dino.texture.height}, dino.position, WHITE);

        DrawRectangle(0, screenHeight - dino.height + 40, screenWidth, 100, Color{77, 255, 136, 255}); // Draw ground
        // // Draw trees
        for (int i = 0; i < MAX_TREES; i++)
        {
            DrawRectangle(trees[i * 2].rec.x, trees[i * 2].rec.y, trees[i * 2].rec.width, trees[i * 2].rec.height, trees[i * 2].color);
            DrawRectangle(trees[i * 2 + 1].rec.x, trees[i * 2 + 1].rec.y, trees[i * 2 + 1].rec.width, trees[i * 2 + 1].rec.height, trees[i * 2 + 1].color);

            DrawTextureRec(trees[i * 2].texture, {0, 0, (float)trees[i * 2].texture.width, (float)trees[i * 2].texture.height}, {trees[i * 2].rec.x - trees[i * 2].texture.width / 2 + 10, trees[i * 2].rec.y + 256}, WHITE);
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

        DrawButton("Quit Game", 20, Color{255, 255, 255, 255}, 200, 40, screenWidth / 2 - 100, screenHeight / 2 + 40, CloseWindow);
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
