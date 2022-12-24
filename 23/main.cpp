#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <set>

using namespace std;

struct elf{
    int row;
    int col;
    bool move;
    int next_row;
    int next_col;
};

void print_grid(vector<vector<bool>> & grid) {
    for (int i=0; i < grid.size(); i++) {
        cout << setw(3) << i << ": ";
        for (int j : grid[i]) {
            if (j) {
                cout << "#";
            } else {
                cout << ".";
            }
        }
        cout << endl;
    }
}

int calc_score(const vector<elf*>& elves, const vector<vector<bool>>& grid) {
    int left = (int)grid.size(), top = (int)grid.size(), bot = 0, right = 0;
    for (auto & elf : elves) {
        if (elf->row < top) {
            top = elf->row;
        }
        if (elf->row > bot) {
            bot = elf->row;
        }
        if (elf->col < left) {
            left = elf->col;
        }
        if (elf->col > right) {
            right = elf->col;
        }
    }
    printf("(%d,%d) to (%d,%d)\n", top, left, bot, right);
    int sum = 0;
    for (int i=top; i <= bot; i++) {
        for (int j=left; j <= right; j++) {
            if (!grid[i][j]) {
                sum++;
            }
        }
    }
    return sum;
}

int run_round(vector<elf*>& elves, vector<vector<bool>>& grid, int round) {
    set<pair<int,int>> planned;
    set<pair<int,int>> banned;
    int stay = 0;
    for (auto & elf : elves) {
        elf->move = false;

        // Check 8 directions
        if (!grid[elf->row - 1][elf->col - 1] &&
            !grid[elf->row - 1][elf->col    ] &&
            !grid[elf->row - 1][elf->col + 1] &&
            !grid[elf->row    ][elf->col - 1] &&
            !grid[elf->row    ][elf->col + 1] &&
            !grid[elf->row + 1][elf->col - 1] &&
            !grid[elf->row + 1][elf->col    ] &&
            !grid[elf->row + 1][elf->col + 1]) {
            continue; // Elf doesn't move if every space around is empty
        }
        
        // Check step preferences
        for (int i=0; i < 4; i++) {
            int r = (i + round) % 4;
            if (r == 0) {
                // Check NW, N, NE
                if (grid[elf->row - 1][elf->col - 1] ||
                    grid[elf->row - 1][elf->col    ] ||
                    grid[elf->row - 1][elf->col + 1]) {
                    continue;
                }
                elf->move = true;
                elf->next_row = elf->row - 1;
                elf->next_col = elf->col;
                break;
            } else if (r == 1) {
                // CHeck SW, S, SE
                if (grid[elf->row + 1][elf->col - 1] ||
                    grid[elf->row + 1][elf->col    ] ||
                    grid[elf->row + 1][elf->col + 1]) {
                    continue;
                }
                elf->move = true;
                elf->next_row = elf->row + 1;
                elf->next_col = elf->col;
                break;
            } else if (r == 2) {
                // Check NW, W, SW
                if (grid[elf->row - 1][elf->col - 1] ||
                    grid[elf->row    ][elf->col - 1] ||
                    grid[elf->row + 1][elf->col - 1]) {
                    continue;
                }
                elf->move = true;
                elf->next_row = elf->row;
                elf->next_col = elf->col - 1;
                break;
            } else {
                // Check NE, E, SE
                if (grid[elf->row - 1][elf->col + 1] ||
                    grid[elf->row    ][elf->col + 1] ||
                    grid[elf->row + 1][elf->col + 1]) {
                    continue;
                }
                elf->move = true;
                elf->next_row = elf->row;
                elf->next_col = elf->col + 1;
                break;
            }
        }
        if (elf->move) {
            if (planned.contains({elf->next_row, elf->next_col})) {
                banned.insert({elf->next_row, elf->next_col});
            } else {
                planned.insert({elf->next_row, elf->next_col});
            }
        }
    }
    for (auto & elf : elves) {
        // Check move target
        if (!elf->move || banned.contains({elf->next_row, elf->next_col})) {
            stay++;
            continue;
        }
        // Update grid
        grid[elf->row][elf->col] = false;
        grid[elf->next_row][elf->next_col] = true;
        // Move elf
        elf->row = elf->next_row;
        elf->col = elf->next_col;
    }
    return stay;
}

int main() {
    cout << "Advent of Code: Day 23" << endl;

    ifstream infile("../23/day23.txt");

    vector<string> grid_strings;
    string input;
    while (getline(infile, input)) {
        grid_strings.push_back(input);
    }
    printf("Grid size: %lu, %lu\n", grid_strings.size(), grid_strings[0].size());

    // Parse Grid
    int GRID_SCALE = (int) grid_strings.size();
    vector<vector<bool>> grid(GRID_SCALE * 3, vector<bool>(GRID_SCALE * 3));
    vector<elf*> elves;
    for (int i=0; i < grid_strings.size(); i++) {
        for (int j=0; j < grid_strings[i].size(); j++) {
            if (grid_strings[i][j] == '#') {
                grid[i + GRID_SCALE][j + GRID_SCALE] = true;
                elf * e = new elf;
                e->row = i + GRID_SCALE;
                e->col = j + GRID_SCALE;
                elves.push_back(e);
            }
        }
    }
    print_grid(grid);

    // Part One
//    const int ROUNDS = 10;
//    for (int r=0; r < ROUNDS; r++) {
//        cout << "Round " << r << endl;
//        run_round(elves, grid, r);
//    }
//    print_grid(grid);
//    int score = calc_score(elves, grid);
//    cout << "Score: " << score << endl;
    // Part Two
    int stayed = 0;
    int round = 0;
    while (stayed < elves.size()) {
        stayed = run_round(elves, grid, round);
        cout << "Round " << round + 1 << ": " << stayed << "/" << elves.size() << endl;
//        print_grid(grid);
        round++;
    }
    print_grid(grid);
    cout << "Ended at round " << round << endl;
    return 0;
    // 1044 too high
}
