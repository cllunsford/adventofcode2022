#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <set>
#include <format>

using namespace std;

typedef vector< vector<int> > Grid;
int COLS = 400;
int X_OFFSET = 300;
int ROWS = 200;

pair<int,int> parsePair(const string& s) {
    pair<int, int> res;
    res.first = stoi(s.substr(0, s.find(',')));
    res.second = stoi(s.substr(s.find(',')+1));
    return res;
}

void printGrid(Grid& grid) {
    for (int y = 0; y < ROWS; y++) {
        cout << setw(3) << y;
        for (int x = 0; x < COLS; x++) {
            int point = grid[x][y];
            if (point == 0) {
                cout << " ";
            } else if (point == 1) {
                cout << "#";
            } else if (point == 2) {
                cout << ".";
            }
        }
        cout << endl;
    }
}

int main() {
    cout << "Advent of Code: Day 14" << endl;

    ifstream infile("../day14.txt");

    // Initialize the grid
    string input;
    set<string> scans;

    while (getline(infile, input)) {
        scans.insert(input);
    }
    cout << scans.size() << endl;

    vector<vector<pair<int,int>>> paths;
    string delim = " -> ";
    int i = 0;
    for (auto & scan : scans) {
        cout << scan << endl;
        vector<pair<int,int>> path;
        size_t start;
        size_t end = 0;
        while ((start = scan.find(delim, end)) != string::npos) {
            path.push_back(parsePair(scan.substr(end, start-end)));
            end = start + delim.length();
        }
        path.push_back(parsePair(scan.substr(end)));
        paths.push_back(path);
        i++;
    }

    int minX = 999, maxX = 0;
    int minY = 999, maxY = 0;
    for (auto & path : paths) {
        for (int j=0; j < path.size(); j++) {
            if (path[j].first > maxX) {
                maxX = path[j].first;
            }
            if (path[j].first < minX) {
                minX = path[j].first;
            }
            if (path[j].second > maxY) {
                maxY = path[j].second;
            }
            if (path[j].second < minY) {
                minY = path[j].second;
            }
        }
    }
    cout << minX << "," << minY << " " << maxX << "," << maxY << endl;

    // Part Two
    pair<int,int> floorLeft(X_OFFSET, maxY + 2);
    pair<int,int> floorRight(X_OFFSET + COLS - 1, maxY + 2);
    vector<pair<int,int>> floor;
    floor.push_back(floorLeft);
    floor.push_back(floorRight);
    paths.push_back(floor);

    Grid grid(COLS,vector<int>(ROWS,0));
    for (auto & path : paths) {
        for (int j = 1; j < path.size(); j++) {
            pair<int,int> start = path[j-1];
            pair<int,int> end = path[j];

            if (start.second == end.second) {
                // horizontal line
                int s, e;
                if (start.first < end.first) {
                    s = start.first;
                    e = end.first;
                } else {
                    s = end.first;
                    e = start.first;
                }
                for (int k = s; k <= e; k++) {
                    grid[k - X_OFFSET][start.second] = 1; // Mark as rock
                }
            } else if (start.first == end.first) {
                // vertical line
                int s, e;
                if (start.second < end.second) {
                    s = start.second;
                    e = end.second;
                } else {
                    s = end.second;
                    e = start.second;
                }
                for (int k = s; k <= e; k++) {
                    grid[start.first - X_OFFSET][k] = 1; // Mark as rock
                }
            }
        }
    }

    printGrid(grid);

    // Part One
    int grains = 0;
//    bool abyss = false;
    bool blocked = false;
    while (!blocked) {
        grains++;
        cout << "Grain: " << grains << endl;
        pair<int,int> grain(500 - X_OFFSET, 0);
        bool settled = false;
        while (!settled) {
//            cout << ".." << grain.first << "," << grain.second << endl;
            // Part One
//            if (grain.second == (ROWS - 1)) {
//                abyss = true;
//                break;
//            }
            if (grid[grain.first][grain.second+1] == 0) {
                grain.second++;
                continue;
            }
            if (grid[grain.first - 1][grain.second + 1] == 0) {
                grain.first--;
                grain.second++;
                continue;
            }
            if (grid[grain.first + 1][grain.second + 1] == 0) {
                grain.first++;
                grain.second++;
                continue;
            }
            settled = true;
            grid[grain.first][grain.second] = 2;
        }
        if (grain.first == 500 - X_OFFSET && grain.second == 0) {
            blocked = true;
        }
    }

    cout << "Grain of sand to fall: " << grains << endl;
    printGrid(grid);
    return 0;
}
