#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <set>

using namespace std;

int main() {
    cout << "Advent of Code: Day 9" << endl;

    ifstream infile("../day9.txt");

//    int NUM_KNOTS = 2; // Part One
    int NUM_KNOTS = 10; // Part Two


    pair<int,int> knots[NUM_KNOTS];
    set<pair<int,int>> visited;
    visited.insert(knots[NUM_KNOTS - 1]);

    string dir, steps;
    while (infile >> dir >> steps) {
        cout << dir << " for " << steps << endl;
        for (int i = 0; i < stoi(steps); i++) {
            printf("Head: %d,%d  Tail: %d,%d\n", knots[0].first, knots[0].second, knots[NUM_KNOTS - 1].first, knots[NUM_KNOTS-1].second);
            // Move the head
            if (dir == "U") {
                knots[0].second++;
            } else if (dir == "R") {
                knots[0].first++;
            } else if (dir == "D") {
                knots[0].second--;
            } else if (dir == "L") {
                knots[0].first--;
            }
            printf("Head to: %d,%d\n", knots[0].first, knots[0].second);

            for (int t=1; t < NUM_KNOTS; t++) {
                pair<int,int> head = knots[t-1];
                pair<int,int> tail = knots[t];
                if (abs(head.first - tail.first) > 1) {
                    tail.first = tail.first + ((head.first - tail.first) / abs(head.first - tail.first));
                    if (abs(head.second - tail.second) > 0) {
                        tail.second = tail.second + ((head.second - tail.second) / abs(head.second - tail.second));
                    }
                }
                if (abs(head.second - tail.second) > 1) {
                    tail.second = tail.second + ((head.second - tail.second) / abs(head.second - tail.second));
                    if (abs(head.first - tail.first) > 0) {
                        tail.first = tail.first + ((head.first - tail.first) / abs(head.first - tail.first));
                    }
                }
                knots[t] = tail;
            }

            visited.insert(knots[NUM_KNOTS - 1]);
        }
    }

    cout << visited.size() << endl;
    return 0;
}
