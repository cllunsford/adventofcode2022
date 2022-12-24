#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <set>

using namespace std;

int ROW_MAX, COL_MAX;

struct blizzard{
    int row{};
    int col{};
    pair<int,int> dir;
};

void print_grid(vector<vector<int>> & grid) {
    for (auto & row : grid) {
        for (auto & col : row) {
            cout << col;
        }
        cout << endl;
    }
}

int find_path(vector<blizzard*> & blizzards, pair<int,int> start, pair<int,int> goal) {
    set<pair<int,int>> candidates;
    candidates.insert(start);
    int t = 0;
    int at_exit = false;
    // Loop until we arrive at the exit
    while (!at_exit) {
        t++;
        // Move blizzards
        vector<vector<int>> grid(ROW_MAX + 1, vector<int>(COL_MAX + 1));
        for (auto & b : blizzards) {
            b->row += b->dir.first;
            b->col += b->dir.second;
            if (b->row == 0) {
                b->row = ROW_MAX - 1;
            } else if (b->row == ROW_MAX) {
                b->row = 1;
            } else if (b->col == COL_MAX) {
                b->col = 1;
            } else if (b->col == 0) {
                b->col = COL_MAX - 1;
            }
            grid[b->row][b->col]++;
        }

//        print_grid(grid);

        // Test moves for each candidate
        set<pair<int,int>> new_candidates;
        for (auto & pos : candidates) {
            // Look up
            if (pos.first - 1 == goal.first && pos.second == goal.second) {
                at_exit = true;
                break;
            } else if (pos.first - 1 > 0 && grid[pos.first - 1][pos.second] == 0) {
                new_candidates.insert({pos.first - 1, pos.second});
            }
            // Look right
            if (pos.first != start.first && pos.second + 1 < COL_MAX && grid[pos.first][pos.second + 1] == 0) {
                new_candidates.insert({pos.first, pos.second + 1});
            }
            // Look down
            if (pos.first + 1 == goal.first && pos.second == goal.second) {
                at_exit = true;
                break;
            } else if (pos.first + 1 < ROW_MAX && grid[pos.first + 1][pos.second] == 0) {
                new_candidates.insert({pos.first + 1, pos.second});
            }
            // Look left
            if (pos.first != start.first && pos.second - 1 > 0 && grid[pos.first][pos.second - 1] == 0) {
                new_candidates.insert({pos.first, pos.second - 1});
            }
            // Wait
            if (grid[pos.first][pos.second] == 0) {
                new_candidates.insert({pos.first, pos.second});
            }
        }

        candidates = new_candidates;
        // Print current paths
        cout << "Step " << t << endl;
        for (auto & c : candidates) {
            cout << "(" << c.first << "," << c.second << ")" << endl;
        }
    }
    return t;
}

int main() {
    cout << "Advent of Code: Day 24" << endl;

    ifstream infile("../24/day24.txt");

    vector<string> grid_strings;
    string input;
    while (getline(infile, input)) {
        grid_strings.push_back(input);
    }
    printf("Grid size: %lu, %lu\n", grid_strings.size(), grid_strings[0].size());

    // Parse Grid
    ROW_MAX = (int) grid_strings.size() - 1;
    COL_MAX = (int) grid_strings[0].size() - 1;
    vector<blizzard*> blizzards;
    for (int i=0; i < grid_strings.size(); i++) {
        for (int j=0; j < grid_strings[i].size(); j++) {
            if (grid_strings[i][j] == '#' || grid_strings[i][j] == '.') {
                continue;
            }
            auto* b = new blizzard;
            b->row = i;
            b->col = j;
            if (grid_strings[i][j] == '<') {
                b->dir = {0, -1};
            } else if (grid_strings[i][j] == '^') {
                b->dir = {-1, 0};
            } else if (grid_strings[i][j] == '>') {
                b->dir = {0, 1};
            } else if (grid_strings[i][j] == 'v') {
                b->dir = {1, 0};
            }
            blizzards.push_back(b);
        }
    }
    cout << "Blizzard count: " << blizzards.size() << endl;

    // Part One
    pair<int,int> goal = {ROW_MAX, COL_MAX - 1};
    int out = find_path(blizzards, {0, 1}, goal);
    cout << "Shortest path " << out << endl;
    // Part Two
    int back = find_path(blizzards, goal, {0, 1});
    int out_again = find_path(blizzards, {0, 1}, goal);

    cout << "Full trip: " << (out + back + out_again) << endl;
    return 0;
}
