#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Reinder eat star fruit
// Retrieve 50 stars minimum

// Calories is puzzle input
// Elves have inventory, separated by blank line
int main() {
    cout << "Advent of Code: Day 1" << endl;
    string a;
    ifstream infile("../day1.txt");

    vector<int> elves;

    int current = 0;
    while (getline(infile, a)) {
        if (a.empty()) {
            elves.push_back(current);
            current = 0;
        } else {
            current = current + stoi(a);
        }
    }
    sort(elves.begin(), elves.end(), greater<>());
    printf("0: %d\n", elves[0]);
    printf("1: %d\n", elves[1]);
    printf("2: %d\n", elves[2]);
    printf("Total: %d\n", (elves[0] + elves[1] + elves[2]));
    return 0;
}
