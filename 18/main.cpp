#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <sstream>

using namespace std;

int GRID_SIZE = 21;
vector<vector<vector<int>>> grid(GRID_SIZE, vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE)));

void vaporizeNeighbors(int x, int y, int z) {
    if (grid[x][y][z] != 1) {
        grid[x][y][z] = 2;
    }
    // Check all sides
    if ((x+1) < GRID_SIZE && grid[x+1][y][z] == 0) {
        vaporizeNeighbors(x+1,y,z);
    }

    if ((x-1) > 0 && grid[x-1][y][z] == 0) {
        vaporizeNeighbors(x-1,y,z);
    }

    if ((y+1) < GRID_SIZE && grid[x][y+1][z] == 0) {
        vaporizeNeighbors(x,y+1,z);
    }

    if ((y-1) > 0 && grid[x][y-1][z] == 0) {
        vaporizeNeighbors(x,y-1,z);
    }

    if ((z+1) < GRID_SIZE && grid[x][y][z+1] == 0) {
        vaporizeNeighbors(x,y,z+1);
    }

    if ((z-1) > 0 && grid[x][y][z-1] == 0) {
        vaporizeNeighbors(x,y,z-1);
    }
}

int main() {
    cout << "Advent of Code: Day 18" << endl;

    ifstream infile("../18/day18.txt");

    string input;
    vector<tuple<int,int,int>> boxes;
    while (getline(infile, input)) {
        stringstream ps(input);
        string tmp;
        vector<int> points;
        while(getline(ps, tmp, ',')) {
            points.push_back(stoi(tmp));
        }
        grid[points[0]][points[1]][points[2]] = 1;
        boxes.emplace_back(points[0], points[1], points[2]);
    }

    // Vaporize Grid to find external points
    vaporizeNeighbors(0,0,0);

    int surfaceArea = 0;
    int externalArea = 0;
    for (auto & box : boxes) {
        int x = get<0>(box);
        int y = get<1>(box);
        int z = get<2>(box);

        // Check all sides
        if ((x+1) == GRID_SIZE || grid[x+1][y][z] == 2) {
            externalArea++;
            surfaceArea++;
        } else if (grid[x+1][y][z] == 0) {
            surfaceArea++;
        }

        if ((x-1) < 0 || grid[x-1][y][z] == 2) {
            externalArea++;
            surfaceArea++;
        } else if (grid[x-1][y][z] == 0) {
            surfaceArea++;
        }

        if ((y+1) == GRID_SIZE || grid[x][y+1][z] == 2) {
            externalArea++;
            surfaceArea++;
        } else if (grid[x][y+1][z] == 0) {
            surfaceArea++;
        }

        if ((y-1) < 0 || grid[x][y-1][z] == 2) {
            externalArea++;
            surfaceArea++;
        } else if (grid[x][y-1][z] == 0) {
            surfaceArea++;
        }

        if ((z+1) == GRID_SIZE || grid[x][y][z+1] == 2) {
            externalArea++;
            surfaceArea++;
        } else if (grid[x][y][z+1] == 0) {
            surfaceArea++;
        }

        if ((z-1) < 0 || grid[x][y][z-1] == 2) {
            externalArea++;
            surfaceArea++;
        } else if (grid[x][y][z-1] == 0) {
            surfaceArea++;
        }
    }
    cout << "Total Surface Area: " << surfaceArea << endl;
    cout << "External Surface Area: " << externalArea << endl;

    return 0;
}