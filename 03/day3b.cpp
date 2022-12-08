#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    cout << "Advent of Code: Day 3" << endl;

    ifstream infile("../day3.txt");

    string line;
    int score = 0;
    string a,b,ab,c;
    int place = 0;
    while (infile >> line) {
        switch (place) {
            case 0:
                a = line;
                sort(a.begin(), a.end());
                printf("%s\n", a.c_str());
                break;
            case 1:
                b = line;
                sort(b.begin(), b.end());
                set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(ab));
                printf("%s\n%s\n", b.c_str(), ab.c_str());
                break;
            case 2:
                c = line;
                string match;
                sort(c.begin(), c.end());
                set_intersection(ab.begin(), ab.end(), c.begin(), c.end(), back_inserter(match));

                printf("%s\n", match.c_str());
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
                ab = "";
                break;
        }
        place = (place + 1) % 3;
    }

    printf("Total: %d", score);
    return 0;
}
