#include <iostream>
#include <fstream>
#include <utility>

using namespace std;

int main() {
    cout << "Advent of Code: Day 8" << endl;

    ifstream infile("../day8.txt");

    // Initialize the grid
    int grid[99][99];
    string input;
    int current = 0;
    while (getline(infile, input)) {
        for (int i=0; i < input.length(); i++) {
            grid[i][current] = std::stoi(input.substr(i,1));
        }
        current++;
    }

    // Part One
    int count = 0;
    for (int x=0; x < 99; x++) {
        for (int y=0; y < 99; y++) {
            // Edges are always visible
            if (x == 0 || x == 98 || y == 0 || y == 98) {
                count++;
                continue;
            }
            int value = grid[x][y];
//            printf("[%d,%d]: %d\n", x, y, value);
            // Look west
            bool n = true, s = true, e = true, w = true;
            for (int west=(x - 1); west >= 0; west--) {
//                printf("Pos[%d,%d]: %d compared to [%d,%d]: %d\n", x, y, value, west, y, grid[west][y]);
                if (value <= grid[west][y]) {
                    w = false;
                    break;
                }
            }
            if (w) { count++; continue; }
            // Look east
            for (int east=(x + 1); east <= 98; east++) {
                if (value <= grid[east][y]) {
                    e = false;
                    break;
                }
            }
            if (e) { count++; continue; }
            // Look north
            for (int north=(y - 1); north >=0; north--) {
                if (value <= grid[x][north]) {
                    n = false;
                    break;
                }
            }
            if (n) { count++; continue; }
            // Look south
            for (int south=(y+1); south <=98; south++) {
                if (value <= grid[x][south]) {
                    s = false;
                    break;
                }
            }
            if (s) { count++; continue; }
        }
    }

    cout << count << endl;

    // Part two
    int highScore = 0;
    for (int x=0; x < 99; x++) {
        for (int y=0; y < 99; y++) {
            // Edges have a 0 score in one direction, which makes scenic score 0
            if (x == 0 || x == 98 || y == 0 || y == 98) {
                continue;
            }

            int value = grid[x][y];
//            printf("[%d,%d]: %d\n", x, y, value);
            int n = 0, s = 0, e = 0, w = 0;
            // Look west
            for (int west=(x - 1); west >= 0; west--) {
                w++;
//                printf("Pos[%d,%d]: %d compared to [%d,%d]: %d\n", x, y, value, west, y, grid[west][y]);
                if (value <= grid[west][y]) {
                    break;
                }
            }
            // Look east
            for (int east=(x + 1); east <= 98; east++) {
                e++;
                if (value <= grid[east][y]) {
                    break;
                }
            }
            // Look north
            for (int north=(y - 1); north >=0; north--) {
                n++;
                if (value <= grid[x][north]) {
                    break;
                }
            }
            // Look south
            for (int south=(y+1); south <=98; south++) {
                s++;
                if (value <= grid[x][south]) {
                    break;
                }
            }
            int localScore = n * s * e * w;
            if (localScore > highScore) {
                highScore = localScore;
            }
        }
    }
    cout << highScore << endl;

    return 0;
}
