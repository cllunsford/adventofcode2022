#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <queue>

using namespace std;

#define ROWS 41
#define COLS 163

int dX[] = { -1, 0, 1, 0 };
int dY[] = { 0, 1, 0, -1 };

// Function to perform the BFS traversal
int bfs(int grid[COLS][ROWS], bool vis[COLS][ROWS], pair<int,int> start, pair<int,int> goal) {
    // Store grid points in bfs queue
    queue<pair<int, int>> q;
    int moves = 0;
    int nodesLeft = 1;
    int nextNodes = 0;
    bool found = false;

    // Push the starting cell onto the queue, mark it as visited
    q.push(start);
    vis[start.first][start.second] = true;

    // Iterate until the queue is empty
    while (!q.empty()) {

        pair<int, int> cell = q.front();
        if (cell == goal) {
            found = true;
            break;
        }
        int x = cell.first;
        int y = cell.second;

//        printf("-> (%d, %d) %d\n", x, y, grid[x][y]);

        q.pop();

        // Visit all 4 adjacent cells
        for (int i = 0; i < 4; i++) {
            int adjX = x + dX[i];
            int adjY = y + dY[i];

            // Validity checks
            // If cell lies out of bounds
            if (adjX < 0 || adjX >= COLS || adjY < 0 || adjY >= ROWS) {
                continue;
            }

            // If cell is already visited
            if (vis[adjX][adjY]) {
                continue;
            }

            // If elevation is within 1
            if (grid[adjX][adjY] > grid[x][y] + 1) {
                continue;
            }

            // Otherwise
            q.push({ adjX, adjY });
            vis[adjX][adjY] = true;
            nextNodes++;
        }
        nodesLeft--;
        if (nodesLeft == 0) {
            nodesLeft = nextNodes;
            nextNodes = 0;
            moves++;
        }
    }
    if (found) {
        return moves;
    }
    return 9999;
}

int main() {
    cout << "Advent of Code: Day 12" << endl;

    ifstream infile("../day12.txt");

    // Initialize the grid
    int grid[COLS][ROWS];
    string input;
    int row = 0;
    pair<int, int> current(0,0);
    pair<int, int> peak(0,0);
    while (getline(infile, input)) {
        for (int i=0; i < input.length(); i++) {
            grid[i][row] = (int) input[i] - 96;
            if (input[i] == 'S') {
                current.first = i;
                current.second = row;
                grid[i][row] = (int) 'a' - 96;
            } else if (input[i] == 'E') {
                peak.first = i;
                peak.second = row;
                grid[i][row] = (int) 'z' - 96;
            }
        }
        row++;
    }

    bool visited[COLS][ROWS];
    memset(visited, false, sizeof visited);

    printf("Current: (%d, %d)\n", current.first, current.second);
    printf("Peak: (%d, %d)\n", peak.first, peak.second);

    int steps = bfs(grid, visited, current, peak);

    // Part One
    cout << "Steps: " << steps << endl;

    // Part Two
    int minSteps = steps;
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            if (grid[i][j] != 1) {
                continue;
            }
            pair<int,int> possible(i, j);
            bool posVisited[COLS][ROWS];
            memset(posVisited, false, sizeof posVisited);
            int posSteps = bfs(grid, posVisited, possible, peak);
//            printf("Possible Run: (%d, %d = %d) %d\n", i, j, grid[i][j], posSteps);
            if (posSteps < minSteps) {
                minSteps = posSteps;
            }
        }
    }
    cout << "Min Steps: " << minSteps << endl;

    return 0;
}
