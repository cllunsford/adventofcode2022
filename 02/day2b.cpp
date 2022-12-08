#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

enum thrown : int {
    Rock = 1,
    Paper = 2,
    Scissors = 3
};

enum outcome : int {
    Lose = 0,
    Draw = 3,
    Win = 6
};

int main() {
    cout << "Advent of Code: Day 2" << endl;

    ifstream infile("../day2.txt");

    string a, b;
    int score = 0;
    while (infile >> a >> b) {
        int round = 0;
        if (b == "X") {
            // Lose
            if (a == "A") {
                // Opponent: Rock
                round = Lose + Scissors;
            } else if (a == "B") {
                // Opponent: Paper
                round = Lose + Rock;
            } else if (a == "C") {
                // Opponent: Scissors
                round = Lose + Paper;
            }
        } else if (b == "Y") {
            // Draw
            if (a == "A") {
                // Opponent: Rock
                round = Draw + Rock;
            } else if (a == "B") {
                // Opponent: Paper
                round = Draw + Paper;
            } else if (a == "C") {
                // Opponent: Scissors
                round = Draw + Scissors;
            }
        } else if (b == "Z") {
            // Win
            if (a == "A") {
                // Opponent: Rock
                round = Win + Paper;
            } else if (a == "B") {
                // Opponent: Paper
                round = Win + Scissors;
            } else if (a == "C") {
                // Opponent: Scissors
                round = Win + Rock;
            }
        }

        score += round;
        printf("%s:%s %d (%d)\n", a.c_str(), b.c_str(), round, score);
    }

    printf("Total: %d", score);
    return 0;
}
