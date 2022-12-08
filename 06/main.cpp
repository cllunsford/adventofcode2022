#include <iostream>
#include <fstream>
#include <set>

using namespace std;

int main() {
    cout << "Advent of Code: Day 6" << endl;

    ifstream infile("../day6.txt");

    //int MARKER_LENGTH = 4; // Part one
    int MARKER_LENGTH = 14; // Part two

    string input;
    while (infile >> input) {
        for (int i = MARKER_LENGTH; i < input.length(); i++) {
            string group_string = input.substr(i-MARKER_LENGTH, MARKER_LENGTH);
            std::set<char> group(group_string.begin(), group_string.end());
            cout << group_string << ": " << group.size() << endl;
            if (group.size() == MARKER_LENGTH) {
                printf("Result: %s %d\n", group_string.c_str(), i);
                return 0;
            }
        }
    }

    return 0;
}
