#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    cout << "Advent of Code: Day 4" << endl;

    ifstream infile("../day4.txt");

    string line;
    int count = 0;
    while (infile >> line) {
        string low = line.substr(0,line.find(','));
        int low_a = stoi(low.substr(0, low.find('-')));
        int low_b = stoi(low.substr(low.find('-') + 1));

        string high = line.substr(line.find(',')+1);
        int high_a = stoi(high.substr(0, high.find('-')));
        int high_b = stoi(high.substr(high.find('-') + 1));

        printf("%s\n", line.c_str());
        // Part One
        //        if ((low_a <= high_a) && (low_b >= high_b)) {
        //            count += 1;
        //        } else if ((high_a <= low_a) && (high_b >= low_b)) {
        //            count += 1;
        //        }
        // Part Two
        if ((low_b >= high_a) && (low_a <= high_a)) {
            count += 1;
        } else if ((high_b >= low_a) && (high_a <= low_a)) {
            count += 1;
        }
    }

    printf("Total: %d", count);
    return 0;
}
