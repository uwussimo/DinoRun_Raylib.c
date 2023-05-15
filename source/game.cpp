#include <iostream>
#include "raylib.h"
#include "../headers/game.h"

using namespace std;

Game::Game(){

}

void Game::InitGame(void)
{
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
}

// Update game (one frame)
void Game::UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P'))
            pause = !pause;

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
                dino.position.y -= 200; // Jump

            else if (dino.position.y < screenHeight - 100)
                dino.position.y += 5;

            // Check Collisions
            for (int i = 0; i < MAX_TREES * 2; i++)
            {
                if (CheckCollisionCircleRec(dino.position, dino.radius, trees[i].rec))
                {
                    gameOver = true;
                    pause = false;
                }
                else if ((treesPos[i / 2].x < dino.position.x) && trees[i / 2].active && !gameOver)
                {
                    score += 100;
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
        DrawFPS(20,100);

        if (pause)
            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, BLACK);
    }
    else
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, WHITE);

    EndDrawing();
}

// Unload game variables
void Game::UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void Game::UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}
