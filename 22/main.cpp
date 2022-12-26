#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

using namespace std;

int FACE_SIZE;
bool CUBE_MODE = false;
struct walker{
    int row;
    int col;
    int dir;
};

string humanize_dir(int dir) {
    if (dir == 0) {
        return "right";
    } else if (dir == 1) {
        return "down";
    } else if (dir == 2) {
        return "left";
    } else if (dir == 3) {
        return "up";
    }
    return "--";
}

string symbolize_dir(int dir) {
    if (dir == 0) {
        return ">";
    } else if (dir == 1) {
        return "v";
    } else if (dir == 2) {
        return "<";
    } else if (dir == 3) {
        return "^";
    }
    return "--";
}

int face(int row, int col) {
    if (row >= 0 && row < FACE_SIZE && col >= FACE_SIZE && col < 2 * FACE_SIZE) {
        return 1;
    } else if (row >= 0 && row < FACE_SIZE && col >= 2 * FACE_SIZE && col < 3 * FACE_SIZE) {
        return 2;
    } else if (row >= FACE_SIZE && row < 2 * FACE_SIZE && col >= FACE_SIZE && col < 2 * FACE_SIZE) {
        return 3;
    } else if (row >= 2 * FACE_SIZE && row < 3 * FACE_SIZE && col >= 0 && col < FACE_SIZE) {
        return 4;
    } else if (row >= 2 * FACE_SIZE && row < 3 * FACE_SIZE && col >= FACE_SIZE && col < 2 * FACE_SIZE) {
        return 5;
    } else if (row >= 3 * FACE_SIZE && row < 4 * FACE_SIZE && col >= 0 && col < FACE_SIZE) {
        return 6;
    }
    return -1;
}

void print_grid(vector<vector<int>> & grid) {
    for (int i=0; i < grid.size(); i++) {
        cout << setw(3) << i << ": ";
        for (int j : grid[i]) {
            if (j == 2) {
                cout << "#";
            } else if (j == 1) {
                cout << ".";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
}

void print_grid_pos(vector<vector<int>> & grid, walker* w, int last_row, int last_col) {
    for (int i=max(0, min(last_row - 1, w->row - 1)); i <= min((int)grid.size() - 1, max(last_row + 1, w->row + 1)); i++) {
        cout << setw(3) << i << ": ";
        for (int j=0; j < grid[i].size(); j++) {
            if (w->row == i && w->col == j) {
                cout << "\x1B[91m" << symbolize_dir(w->dir) << "\033[0m";
            } else if (last_row == i && last_col == j) {
                cout << "\x1B[92m" << symbolize_dir(w->dir) << "\033[0m";
            } else if (grid[i][j] == 2) {
                cout << "#";
            } else if (grid[i][j] == 1) {
                switch (face(i, j)) {
                    case 1:
                        cout << "\x1B[33m.\033[0m";
                        break;
                    case 2:
                        cout << "\x1B[34m.\033[0m";
                        break;
                    case 3:
                        cout << "\x1B[35m.\033[0m";
                        break;
                    case 4:
                        cout << "\x1B[36m.\033[0m";
                        break;
                    case 5:
                        cout << "\x1B[37m.\033[0m";
                        break;
                    case 6:
                        cout << "\x1B[32m.\033[0m";
                        break;
                    default:
                        cout << ".";
                        break;
                }
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void next_step(vector<vector<int>> & grid, walker* w) {
    if (w->dir == 0) {
        if (w->col + 1 >= grid[w->row].size() || grid[w->row][w->col + 1] == 0) {
            if (CUBE_MODE) {
                int f = face(w->row, w->col);
                // Running off right only happens from: 2,3,5,6
                if (f == 2) {
                    // in 5 upside-down at row (inverted), col opposite side
                    int new_row = (3 * FACE_SIZE) - 1 - w->row;
                    int new_col = (2 * FACE_SIZE) - 1;
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 2; // facing left
                        return;
                    }
                } else if (f == 3) {
                    // in 2 from bottom
                    int new_row = FACE_SIZE - 1;
                    int new_col = (w->row - FACE_SIZE) + (2 * FACE_SIZE);
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 3; // facing up
                        return;
                    }
                } else if (f == 5) {
                    // in 2 upside-down at row (inverted), col opposite side
                    int new_row = (3 * FACE_SIZE) - 1 - w->row;
                    int new_col = (3 * FACE_SIZE) - 1;
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 2; // facing left
                        return;
                    }
                } else if (f == 6) {
                    // in 5 from bottom
                    int new_row = (3 * FACE_SIZE) - 1;
                    int new_col = (w->row - (3 * FACE_SIZE)) + (FACE_SIZE);
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 3; // facing up
                        return;
                    }
                }
                return;
            } else {
                for (int i=0; i < grid[w->row].size(); i++) {
                    if (grid[w->row][i] == 0) {
                        continue;
                    } else if (grid[w->row][i] == 1) {
                        w->col = i;
                        return;
                    } else {
                        return;
                    }
                }
            }
        } else if (grid[w->row][w->col + 1] == 2) {
            return;
        } else if (grid[w->row][w->col + 1] == 1) {
            w->col++;
            return;
        }
    } else if (w->dir == 1) {
        if (w->row + 1 >= grid.size() || grid[w->row + 1][w->col] == 0) {
            if (CUBE_MODE) {
                // Running off down only happens from 2,5,6
                int f = face(w->row, w->col);
                if (f == 2) {
                    // in 3 from right
                    int new_row = w->col - (2 * FACE_SIZE) + (FACE_SIZE);
                    int new_col = (2 * FACE_SIZE) - 1;
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 2; // facing left
                        return;
                    }
                } else if (f == 5) {
                    // in 6 from right
                    int new_row = w->col - (1 * FACE_SIZE) + (3 * FACE_SIZE);
                    int new_col = (1 * FACE_SIZE) - 1;
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 2; // facing left
                        return;
                    }
                } else if (f == 6) {
                    // in 2 from top
                    int new_row = 0;
                    int new_col = w->col + (2 * FACE_SIZE);
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 1; // still facing down
                        return;
                    }
                }
                return;
            } else {
                for (int i=0; i < grid.size(); i++) {
                    if (grid[i][w->col] == 0) {
                        continue;
                    } else if (grid[i][w->col] == 1) {
                        w->row = i;
                        return;
                    } else {
                        return;
                    }
                }
            }
        } else if (grid[w->row + 1][w->col] == 2) {
            return;
        } else if (grid[w->row + 1][w->col] == 1) {
            w->row++;
            return;
        }
    } else if (w->dir == 2) {
        if (w->col - 1 < 0 || grid[w->row][w->col - 1] == 0) {
            if (CUBE_MODE) {
                // Running off left happens from 1,3,4,6
                int f = face(w->row, w->col);
                if (f == 1) {
                    // in 4 upside-down at row (inverted), col opposite side
                    int new_row = (3 * FACE_SIZE) - 1 - w->row;
                    int new_col = 0;
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 0; // facing right
                        return;
                    }
                } else if (f == 3) {
                    // in 4 from top
                    int new_row = 2 * FACE_SIZE;
                    int new_col = w->row - FACE_SIZE;
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 1; // facing down
                        return;
                    }
                } else if (f == 4) {
                    // in 1 from left
                    int new_row = (3 * FACE_SIZE) - 1 - w->row;
                    int new_col = FACE_SIZE;
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 0; // facing right
                        return;
                    }
                } else if (f == 6) {
                    // in 1 from top
                    int new_row = 0;
                    int new_col = w->row - (3 * FACE_SIZE) + (FACE_SIZE);
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 1; // facing down
                        return;
                    }
                }
                return;
            } else {
                for (int i = (int) grid[w->row].size() - 1; i >= 0; i--) {
                    if (grid[w->row][i] == 0) {
                        continue;
                    } else if (grid[w->row][i] == 1) {
                        w->col = i;
                        return;
                    } else {
                        return;
                    }
                }
            }
        } else if (grid[w->row][w->col - 1] == 2) {
            return;
        } else if (grid[w->row][w->col - 1] == 1) {
            w->col--;
            return;
        }
    } else if (w->dir == 3) {
        if (w->row - 1 < 0 || grid[w->row - 1][w->col] == 0) {
            if (CUBE_MODE) {
                // Running off up happens from 1,2,4
                int f = face(w->row, w->col);
                if (f == 1) {
                    // in 6 from left
                    int new_row = w->col - FACE_SIZE + (3 * FACE_SIZE);
                    int new_col = 0;
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 0; // facing right
                        return;
                    }
                } else if (f == 2) {
                    // in 6 from bottom
                    int new_row = (4 * FACE_SIZE) - 1;
                    int new_col = w->col - (2 * FACE_SIZE);
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 3; // still facing up
                        return;
                    }
                } else if (f == 4) {
                    // in 3 from left
                    int new_row = w->col + FACE_SIZE;
                    int new_col = FACE_SIZE;
                    if (grid[new_row][new_col] == 1) {
                        w->row = new_row;
                        w->col = new_col;
                        w->dir = 0; // facing right
                        return;
                    }
                }
                return;
            } else {
                for (int i = (int) grid.size() - 1; i >= 0; i--) {
                    if (grid[i][w->col] == 0) {
                        continue;
                    } else if (grid[i][w->col] == 1) {
                        w->row = i;
                        return;
                    } else {
                        return;
                    }
                }
            }
        } else if (grid[w->row - 1][w->col] == 2) {
            return;
        } else if (grid[w->row - 1][w->col] == 1) {
            w->row--;
            return;
        }
    }
}

int calc_score(walker *w) {
    return ((w->row + 1) * 1000) + ((w->col + 1) * 4) + w->dir;
}

int main() {
    cout << "Advent of Code: Day 22" << endl;

//    ifstream infile("../22/day22.sample.txt");
//    FACE_SIZE = 4;

    ifstream infile("../22/day22.txt");
    FACE_SIZE = 50;
    CUBE_MODE = true;

    vector<string> grid_strings;
    int longest_line = 0;
    string instructions;
    string input;
    while (getline(infile, input)) {
        if (input.empty()) {
            getline(infile, input);
            instructions = input;
        } else {
            if (input.size() > longest_line) {
                longest_line = (int) input.size();
            }
            grid_strings.push_back(input);
        }
    }
    printf("Grid rows: %lu, cols %d\n", grid_strings.size(), longest_line);
    printf("Instructions: %s\n", instructions.c_str());

    // Parse Grid
    vector<vector<int>> grid(grid_strings.size(), vector<int>(longest_line));
    for (int i=0; i < grid_strings.size(); i++) {
        for (int j=0; j < grid_strings[i].size(); j++) {
            if (grid_strings[i][j] == ' ') {
                grid[i][j] = 0;
            } else if (grid_strings[i][j] == '.') {
                grid[i][j] = 1;
            } else if (grid_strings[i][j] == '#') {
                grid[i][j] = 2;
            }
        }
    }
    print_grid(grid);

    // Parse Instructions
    vector<string> path;
    string next_num;
    for (auto & c : instructions) {
        if (c == 'R') {
            path.push_back(next_num);
            next_num = "";
            path.emplace_back("R");
        } else if (c == 'L') {
            path.push_back(next_num);
            next_num = "";
            path.emplace_back("L");
        } else {
            next_num += c;
        }
    }
    path.push_back(next_num);

    // Walk
    auto* w = new walker;
    w->row = 0;
    w->col = 50;
    w->dir = 0;

    cout << "Starting pos: " << w->row << "," << w->col << endl;
    for (auto & step : path) {
//        int a = cin.get(); // Uncomment to step through each movement by hitting return

        cout << "Next: " << step << " " << humanize_dir(w->dir) << endl;
        if (step == "R") {
            w->dir = (w->dir + 1) % 4;
        } else if (step == "L") {
            if (w->dir == 0) {
                w->dir = 3;
            } else {
                w->dir--;
            }
        } else {
            int count = stoi(step);
            int last_row = w->row;
            int last_col = w->col;
            for (int n=0; n < count; n++) {
                next_step(grid, w);
            }
            print_grid_pos(grid, w, last_row, last_col);
        }
        cout << w->row << "," << w->col << " " << humanize_dir(w->dir) << endl;
    }

    cout << "Final Position: " << w->row << "," << w->col << endl;
    cout << "Password: " << calc_score(w) << endl;
    //  56306 too low
    //  60000 too low
    // 100000 too low
    return 0;
}
