#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <cmath>

using namespace std;

struct monkey{
    int index;
    vector<int64_t> items;
    string opType;
    int opFactor;
    int divisibleBy;
    int trueTarget;
    int falseTarget;
    int64_t touches;
};

void printMonkeys(vector<monkey *> monkeys, bool verbose) {
    for (int i=0; i < monkeys.size(); i++) {
        monkey *m = monkeys[i];
        m->index = i;

        ostringstream stream;
        if(!m->items.empty()){
            copy(m->items.begin(), m->items.end()-1, ostream_iterator<int>(stream,", "));
            stream << m->items.back();
        }
        cout << i << ": " << stream.str() << endl;
        if (verbose) {
            cout << " Op: " << m->opType << " [ " << m->opFactor << " ] " << endl;
            cout << " Divis: " << m->divisibleBy << " (" << m->trueTarget << "," << m->falseTarget << ")" << endl;
            cout << " Touches: " << m->touches << endl;
        }
    }
}

int main() {
    cout << "Advent of Code: Day 11" << endl;

    ifstream infile("../day11.txt");

    vector<monkey *> monkeys;
    string input;
    while (getline(infile, input)) {
        // New Monkey
        monkey *m;
        if (input.starts_with("Monkey")) {
            m = new monkey;
        }

        // Items
        string itemList, tmp;
        vector<int64_t> items;
        getline(infile, input);
        itemList = input.substr(input.find(':') + 2);
        if (itemList != "[]") {
            stringstream itemStream(itemList);
            while(getline(itemStream, tmp, ',')) {
                items.push_back(stoi(tmp));
            }
        }
        m->items = items;

        // Operation
        string opString;
        getline(infile, input);
        opString = input.substr(input.find('=') + 1);
        stringstream opSs(opString);
        string old,op,value;
        opSs >> old >> op >> value;
        if (value == "old") {
            m->opType = "square";
            m->opFactor = 0;
        } else {
            m->opType = op;
            m->opFactor = stoi(value);
        }

        // Divisibility
        string divString;
        getline(infile, input);
        divString = input.substr(input.find(" by ") + 4);
        m->divisibleBy = stoi(divString);

        // If true
        getline(infile, input);
        m->trueTarget = input.back() - '0';

        // If false
        getline(infile, input);
        m->falseTarget = input.back() - '0';

        // Empty line
        getline(infile, input);
        monkeys.push_back(m);
    }

    printMonkeys(monkeys, true);
    // Run rounds
    for (int r = 0; r < 10000; r++) {
        cout << "Round " << r << endl;
        for (monkey *m : monkeys) {
//            cout << "Monkey " << m->index << endl;
            for (int64_t i : m->items) {
                m->touches++;
                // Perform op
                int64_t value;
                if (m->opType == "square") {
                    value = i * i;
                    printf("Square %lld = %lld\n", i, value);
                } else if (m->opType == "*") {
                    value = i * m->opFactor;
                    printf("%lld * %d = %lld\n", i, m->opFactor, value);
                } else if (m->opType == "+") {
                    value = i + m->opFactor;
                    printf("%lld + %d = %lld\n", i, m->opFactor, value);
                } else {
                    value = 0;
                }

                // Reduce worry
                // Part One
//                int postWorry = floor(value / 3);
                // Part Two
                int64_t postWorry = value % 9699690; // magic number is product of all divisibleBy factors

                // Test divisibility
                if (postWorry % m->divisibleBy == 0) {
                    monkeys[m->trueTarget]->items.push_back(postWorry);
                } else {
                    monkeys[m->falseTarget]->items.push_back(postWorry);
                }
            }
            m->items.clear();
//            printMonkeys(monkeys, false);
        }
//        printMonkeys(monkeys, false);
    }

    printMonkeys(monkeys, true);

    // Part One
    vector<int64_t> touches;
    for (auto & m : monkeys) {
        touches.push_back(m->touches);
    }
    sort(touches.begin(), touches.end(), greater<>());
    cout << touches[0] << ", " << touches[1] << endl;
    cout << "Monkey Business: " << (touches[0] * touches[1]) << endl;

    return 0;
}
