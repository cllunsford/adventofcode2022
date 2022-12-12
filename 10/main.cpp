#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

using namespace std;

// Part One
int checkStrength(int cycle, int reg) {
    if ((cycle + 20) % 40 != 0) {
        return 0;
    }
    cout << "Strength " << cycle << endl;
    return (cycle * reg);
}

// Part Two
string drawPixel(int cycle, int reg) {
    int subcycle = cycle % 40;
    if (abs(subcycle - reg) <= 1) {
        return "#";
    }
    return ".";
}

void printCrt(const string& crt) {
    cout << crt.substr(0,40) << endl;
    cout << crt.substr(40,40) << endl;
    cout << crt.substr(80,40) << endl;
    cout << crt.substr(120,40) << endl;
    cout << crt.substr(160,40) << endl;
    cout << crt.substr(200,40) << endl;
}

void debugCrt(string crt, int cycle, int reg) {

}

int main() {
    cout << "Advent of Code: Day 10" << endl;

    ifstream infile("../day10.txt");

    string crt;
    int cycle = 0;
    int reg = 1;
    int total = 0;
    string input;
    while (getline(infile, input)) {
        stringstream ss(input);
        string op,value;
        ss >> op >> value;
        cout << op << " " << value << endl;
        if (op == "noop") {
            total += checkStrength( cycle + 1, reg);
            crt += drawPixel(cycle, reg);
        } else if (op == "addx") {
            total += checkStrength( cycle + 1, reg);
            crt += drawPixel(cycle, reg);
            cycle++;
            total += checkStrength( cycle + 1, reg);
            crt += drawPixel(cycle, reg);
            reg = reg + stoi(value);
        }
        cout << cycle << " " << reg << endl;
        cycle++;
    }

    cout << total << endl;
    printCrt(crt);
    return 0;
}
