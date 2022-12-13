#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

using namespace std;

bool compareStrings(string left, string right) {
    if (left == right) {
        return false; // need this for custom comparator
    }
    cout << endl << "Compare: " << endl;
    cout << left << endl;
    cout << right << endl;
    int li = 0, ri = 0;
    vector<string> lifts;
    while (li < left.length() && ri < right.length()) {
        if (left[li] == right[ri]) { // If they're the same, continue
//            cout << " " << left[li] << " = " << right[ri] << endl;
            li++;
            ri++;
            continue;
        } else if (right[ri] == ']') { // left is longer
            if (!lifts.empty() && lifts.back() == "l") { // left is lifted
                lifts.pop_back();
                continue;
            } else {
                cout << "left is longer" << endl;
                return false;
            }
        } else if (left[li] == ']') { // right is longer
            if (!lifts.empty() && lifts.back() == "r") { // right is lifted
                lifts.pop_back();
                continue;
            } else {
                cout << "left is shorter" << endl;
                return true;
            }
        } else if (left[li] == '[') { // If left starts array
            li++;
            lifts.emplace_back("r");
            cout << "lift right" << endl;
            continue;
        } else if (right[ri] == '[') { // If right starts array
            ri++;
            lifts.emplace_back("l");
            cout << "lift left" << endl;
            continue;
        } else {
            // Account for 10s
            int leftInt, rightInt;
            if (left[li + 1] == '0') {
                leftInt = 10;
                li++;
            } else {
                leftInt = left[li] - '0';
            }
            if (right[ri + 1] == '0') {
                rightInt = 10;
                ri++;
            } else {
                rightInt = right[ri] - '0';
            }
            if (leftInt < rightInt) {
                cout << " " << leftInt << " < " << rightInt << endl;
                return true;
            } else {
                cout << " " << leftInt << " > " << rightInt << endl;
                return false;
            }
        }
    }
    return true;
}

struct signalComparator {
    bool operator()(const std::string& a, const std::string& b) {
        bool res = compareStrings(a, b);
        cout << res << endl;
        return res;
    }
};

int main() {
    cout << "Advent of Code: Day 13" << endl;

    ifstream infile("../day13.txt");

    // Initialize the grid
    string input;
    int score = 0;
    int index = 1;
    string leftS;
    string rightS;
    vector<string> signals;
    cout << index << ": " << endl;
    while (getline(infile, input)) {
        if (input.empty()) {
            index++;
            cout << index << ": " << endl;
        } else if (leftS.empty()) {
            leftS = input;
            cout << " " << leftS << endl;
            signals.push_back(input);
        } else if (rightS.empty()){
            rightS = input;
            cout << " " << rightS << endl;
            signals.push_back(input);
            bool compare = compareStrings(leftS, rightS);
            if (compare) {
                score += index;
            }
            cout << " => " << compare << endl << endl;
            leftS = "";
            rightS = "";
        }
    }

    cout << "Part One Score: " << score << endl;

    // Part Two
    signals.emplace_back("[[2]]");
    signals.emplace_back("[[6]]");
    sort(signals.begin(), signals.end(), signalComparator());

    cout << "Sorted: " << signals.size() << endl;
    int indexOne = 0;
    int indexTwo = 0;
    for (int i = 0; i < signals.size(); i++) {
        cout << i + 1 << ": " << signals[i] << endl;
        if (signals[i] == "[[2]]" || signals[i] == "[[6]]") {
            if (indexOne == 0) {
                indexOne = i + 1;
            } else {
                indexTwo = i + 1;
            }
        }
    }

    cout << "Decoder Key: " << (indexOne * indexTwo) << endl;
    return 0;
}
