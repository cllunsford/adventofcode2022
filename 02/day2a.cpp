#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    cout << "Advent of Code: Day 2" << endl;

    ifstream infile("../day2.txt");

    string a, b;
    int score = 0;
    while (infile >> a >> b) {
        int round = 0;
        if (b == "X") {
            // Rock
            round += 1;
            if (a == "A") {
                // Opponent: Rock
                round += 3;
            } else if (a == "B") {
                // Opponent: Paper
                round += 0;
            } else if (a == "C") {
                // Opponent: Scissors
                round += 6;
            }
        } else if (b == "Y") {
            // Paper
            round += 2;
            if (a == "A") {
                // Opponent: Rock
                round += 6;
            } else if (a == "B") {
                // Opponent: Paper
                round += 3;
            } else if (a == "C") {
                // Opponent: Scissors
                round += 0;
            }
        } else if (b == "Z") {
            // Scissors
            round += 3;
            if (a == "A") {
                // Opponent: Rock
                round += 0;
            } else if (a == "B") {
                // Opponent: Paper
                round += 6;
            } else if (a == "C") {
                // Opponent: Scissors
                round += 3;
            }
        }

        score += round;
        printf("%s:%s %d (%d)\n", a.c_str(), b.c_str(), round, score);
    }

    printf("Total: %d", score);
    return 0;
}
