#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    cout << "Advent of Code: Day 3" << endl;

    ifstream infile("../day3.txt");

    string line;
    int score = 0;
    while (infile >> line) {
        string a = line.substr(0, line.length()/2);
        string b = line.substr(line.length()/2);

        sort(a.begin(), a.end());
        sort(b.begin(), b.end());

        string match;
        set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(match));
        char letter = match.at(0);
        int base = (int) letter;
        int value;
        if (base < 96) {
            value = base - 38;
        } else {
            value = base - 96;
        }

        score += value;
        printf("%c %d\n", letter, value);
    }

    printf("Total: %d", score);
    return 0;
}
