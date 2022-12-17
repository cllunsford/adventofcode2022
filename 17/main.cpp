#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <set>
#include <numeric>

using namespace std;

vector<vector<int>> chamber;

vector<pair<long,int>> shapeCoords(const string& shape, long height) {
    if (shape == "minus") {
        return {
                {height + 4, 2},
                {height + 4, 3},
                {height + 4, 4},
                {height + 4, 5}
        };
    } else if (shape == "plus") {
        return {
                {height + 4, 3},
                {height + 5, 2},
                {height + 5, 3},
                {height + 5, 4},
                {height + 6, 3}
        };
    } else if (shape == "angle") {
        return {
                {height + 4, 2},
                {height + 4, 3},
                {height + 4, 4},
                {height + 5, 4},
                {height + 6, 4}
        };
    } else if (shape == "tower") {
        return {
                {height + 4, 2},
                {height + 5, 2},
                {height + 6, 2},
                {height + 7, 2}
        };
    } else {
        return {
                {height + 4, 2},
                {height + 4, 3},
                {height + 5, 2},
                {height + 5, 3}
        };
    }
}

vector<pair<long,int>> shiftShape(vector<pair<long,int>> shape, int drop, int right) {
    for (auto & elem : shape) {
        elem.first -= drop;
        elem.second += right;
    }
    return shape;
}

bool checkCollision(const vector<pair<long,int>>& shape) {
    for (auto & elem : shape) {
        // Wall collision
        if (elem.second < 0 || elem.second > 6) {
            return true;
        }
        // Floor check
        if (elem.first < 0) {
            return true;
        }
        // Rock collision
        if (chamber[elem.first][elem.second] == 1) {
            return true;
        }
    }
    return false;
}

void freezeRock(const vector<pair<long,int>>& shape) {
    for (auto & elem : shape) {
        chamber[elem.first][elem.second] = 1;
    }
}

long updateHeight(long height) {
    for (long i=height + 1; i < chamber.size(); i++) {
        if (i < 1) {
            continue;
        }
        long sum = reduce(chamber[i].begin(), chamber[i].end());
        if (sum > 0) {
            continue;
        }
        return i-1;
    }
    return height;
}

int main() {
    cout << "Advent of Code: Day 17" << endl;

    ifstream infile("../17/day17.txt");

    string input;
    set<string> scans;

    getline(infile, input);

    cout << "Instructions: " << input.length() << endl;

    long jet = 0;
    long height = -1;
    vector<string> shapes = { "minus", "plus", "angle", "tower", "square" };
    chamber = {
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0},
    };

    // Part One
//    long NUM_ROCKS = 2022;
    // Part Two
    long NUM_ROCKS = 1000000000000;
    long patternHeight = 0;
    long patternRocks = 0;
    long lastPatternHeight = 0;
    long lastPatternRocks = 0;
    long bonusHeight = 0;
    for (long i=1; i <= NUM_ROCKS; i++) {
        patternRocks++;

        string s = shapes[(i - 1) % shapes.size()];
        vector<pair<long,int>> shape = shapeCoords(s, height);
//        cout << "Rock " << i << ", shape: " << s << endl;
//        cout << "Start pos: " << shape[0].first << "," << shape[0].second << endl;
        bool stopped = false;
        while (!stopped) {
            auto ghost = vector(shape);

            // Jet push
            char dir = input[jet % input.size()];
            if (dir == '<') {
//                cout << "Left" << endl;
                ghost = shiftShape(ghost, 0, -1);
                if (!checkCollision(ghost)) {
                    shape = shiftShape(shape, 0, -1);
//                    cout << "Left pos: " << shape[0].first << "," << shape[0].second << endl;
                }
            } else if (dir == '>') {
//                cout << "Right" << endl;
                ghost = shiftShape(ghost, 0, 1);
                if (!checkCollision(ghost)) {
                    shape = shiftShape(shape, 0, 1);
//                    cout << "Right pos: " << shape[0].first << "," << shape[0].second << endl;
                }
            }

            // Fall
            ghost = shiftShape(shape, 1, 0);
            if (!checkCollision(ghost)) {
                shape = shiftShape(shape, 1, 0);
//                cout << "Drop pos: " << shape[0].first << "," << shape[0].second << endl;
            } else {
                stopped = true;
                freezeRock(shape);
                height = updateHeight(height);
//                cout << "Final pos: " << shape[0].first << "," << shape[0].second << endl;
            }

            jet++;
            if (jet % (input.size() * shapes.size()) == 0) {
                patternHeight = (height - patternHeight);
                cout << "Jet reset at " << jet << " [" << height << "]" << endl;
                cout << "Pattern height: " << patternHeight << endl;

                // Rock skip
                if (lastPatternHeight == patternHeight && lastPatternRocks == patternRocks) {
                    // We found our pattern
                    cout << "Pattern found. Height: " << patternHeight << " rocks: " << patternRocks << endl;
                    long repeats = floor((NUM_ROCKS - i) / patternRocks);
                    bonusHeight += patternHeight * repeats;
                    i += patternRocks * repeats;
                    cout << "Skip to rock: " << i << " [" << height + bonusHeight + 1 << "]" << endl;
                }

                // Pattern reset
                lastPatternHeight = patternHeight;
                lastPatternRocks = patternRocks;
                jet = 0;
                patternRocks = 0;
                patternHeight = height;
            }
        }

        // Make sure there's space in the chamber
        while (chamber.size() < height + 10) {
            chamber.push_back({0,0,0,0,0,0,0});
        }
//        cout << "Height: " << height << endl;
    }

    cout << "Final Height: " << height + 1 + bonusHeight << endl;
    return 0;
}