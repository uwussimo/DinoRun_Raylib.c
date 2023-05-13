/**
 * @file utils.h
 * @author Mukhammadyusuf Abdurakhimov <mabdurakhimov26@my.whitworth.edu>
 * @brief Utility functions for the game
 * @version 0.1
 * @date 2023-05-09
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include "raylib.h"

using namespace std;

/**
 * @brief Draw a text in the center of the screen
 *
 * @param text - text to be drawn
 * @param fontSize - size of the font
 * @param color - color of the text
 * @param width - width of the screen
 * @param height - height of the screen
 */
void DrawCentralizedText(string text, int fontSize, Color color, int width, int height)
{
    int xCenter = width / 2;
    int yCenter = height / 2;
    DrawText(text.c_str(),
             xCenter - MeasureText(text.c_str(), fontSize) / 2,
             yCenter - 50,
             fontSize,
             color);
}

/**
 * @brief Draw a button with text in the center of the button and a callback function to be called when the button is clicked
 *
 * @param text - text to be drawn
 * @param fontSize - size of the font
 * @param color - color of the text
 * @param width - width of the button
 * @param height - height of the button
 * @param x - x position of the button
 * @param y - y position of the button
 * @param callback - callback function to be called when the button is clicked
 *
 * @ref callback function: https://cplusplus.com/forum/general/47842/
 */
void DrawButton(string text, int fontSize, Color color, int width, int height, int x, int y, void (*callback)() = nullptr)
{
    // draw rectangle using corresponding width and height and x and y
    DrawRectangle(x, y, width, height, color);

    // use x,y to draw text in the center of the rectangle
    DrawText(text.c_str(),
             x + width / 2 - MeasureText(text.c_str(), fontSize) / 2,
             y + height / 2 - fontSize / 2,
             fontSize,
             BLACK);

    // check if the button is clicked
    if (CheckCollisionPointRec(GetMousePosition(), {(float)x, (float)y, (float)width, (float)height}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // change color of the button to indicate that it is clicked
        DrawRectangle(x, y, width, height, WHITE);
        cout << "Button clicked: " << text << endl;
        if (callback != nullptr) // if callback is not null, call the callback function
            callback();          // call the callback function
    }
}

#endif // UTIL_H