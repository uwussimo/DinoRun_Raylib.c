#include <iostream>
#include "raylib.h"
#include "fstream"
#include "string"
#include "vector"
#include "../headers/game.h"
using namespace std;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    Game game;

    ifstream file;
    file.open("../data/stats.csv");
    vector<vector<string>> stats;
    string line;
    while (getline(file, line))
    {
        vector<string> row;
        string word = "";
        for (auto x : line) // for each character in line
        {
            if (x == ',') // if comma, push word to row and reset word
            {
                row.push_back(word); //  push word to row
                word = "";           // reset word
            }
            else
            {
                word = word + x; // append character to word
            }
        }
        row.push_back(word);  // push last word to row
        stats.push_back(row); // push row to stats
    }

    // print stats
    for (int i = 0; i < stats.size(); i++)
    {
        for (int j = 0; j < stats[i].size(); j++)
        {
            cout << stats[i][j] << " ";
        }
        cout << endl;
    }

    file.close();

    game.stats = stats;

    game.gameLoop(); // Initialize game

    if (stoi(game.stats[0][1]) > stoi(stats[0][1]))
    {
        ofstream file;                  // write stats to file
        file.open("../data/stats.csv"); // open file

        file << "high score," << game.stats[0][1] << endl;
        file << "average score," << game.stats[1][1] << endl;
        file << "avarage score per session," << game.stats[2][1] << endl;
        file << "number of sessions," << game.stats[3][1] << endl;
        file << "number of attempts," << game.stats[4][1] << endl;

        file.close();
    }

    cout << "after game loop" << endl;
    system("pause");
    return 0;
}
