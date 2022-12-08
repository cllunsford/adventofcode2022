#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

string printStacks(vector<string> stacks[]) {
    string out;
    for (int i = 1; i < 10; i++) {
        if (size(stacks[i]) == 0) {
            out += "-";
        } else {
            out += stacks[i].back();
        }
    }
    return out;
}

int main() {
    cout << "Advent of Code: Day 5" << endl;

    ifstream infile("../day5.txt");

    //    [G]         [P]         [M]
    //    [V]     [M] [W] [S]     [Q]
    //    [N]     [N] [G] [H]     [T] [F]
    //    [J]     [W] [V] [Q] [W] [F] [P]
    //[C] [H]     [T] [T] [G] [B] [Z] [B]
    //[S] [W] [S] [L] [F] [B] [P] [C] [H]
    //[G] [M] [Q] [S] [Z] [T] [J] [D] [S]
    //[B] [T] [M] [B] [J] [C] [T] [G] [N]
    // 1   2   3   4   5   6   7   8   9
    vector<string> stacks[10];
    stacks[1].emplace_back("B");
    stacks[1].emplace_back("G");
    stacks[1].emplace_back("S");
    stacks[1].emplace_back("C");

    stacks[2].emplace_back("T");
    stacks[2].emplace_back("M");
    stacks[2].emplace_back("W");
    stacks[2].emplace_back("H");
    stacks[2].emplace_back("J");
    stacks[2].emplace_back("N");
    stacks[2].emplace_back("V");
    stacks[2].emplace_back("G");

    stacks[3].emplace_back("M");
    stacks[3].emplace_back("Q");
    stacks[3].emplace_back("S");

    stacks[4].emplace_back("B");
    stacks[4].emplace_back("S");
    stacks[4].emplace_back("L");
    stacks[4].emplace_back("T");
    stacks[4].emplace_back("W");
    stacks[4].emplace_back("N");
    stacks[4].emplace_back("M");

    stacks[5].emplace_back("J");
    stacks[5].emplace_back("Z");
    stacks[5].emplace_back("F");
    stacks[5].emplace_back("T");
    stacks[5].emplace_back("V");
    stacks[5].emplace_back("G");
    stacks[5].emplace_back("W");
    stacks[5].emplace_back("P");

    stacks[6].emplace_back("C");
    stacks[6].emplace_back("T");
    stacks[6].emplace_back("B");
    stacks[6].emplace_back("G");
    stacks[6].emplace_back("Q");
    stacks[6].emplace_back("H");
    stacks[6].emplace_back("S");

    stacks[7].emplace_back("T");
    stacks[7].emplace_back("J");
    stacks[7].emplace_back("P");
    stacks[7].emplace_back("B");
    stacks[7].emplace_back("W");

    stacks[8].emplace_back("G");
    stacks[8].emplace_back("D");
    stacks[8].emplace_back("C");
    stacks[8].emplace_back("Z");
    stacks[8].emplace_back("F");
    stacks[8].emplace_back("T");
    stacks[8].emplace_back("Q");
    stacks[8].emplace_back("M");

    stacks[9].emplace_back("N");
    stacks[9].emplace_back("S");
    stacks[9].emplace_back("H");
    stacks[9].emplace_back("B");
    stacks[9].emplace_back("P");
    stacks[9].emplace_back("F");

    string move,count_s,from,source_s,to,dest_s,result;
    while (infile >> move >> count_s >> from >> source_s >> to >> dest_s) {
        int count = stoi(count_s);
        int source = stoi(source_s);
        int dest = stoi(dest_s);
        printf("Move [%d] from [%d] to [%d]\n", count, source, dest);

        vector<string> crane;
        for (int i = 0; i < count; i++) {
            // 9000 - Part One
//            string crate = stacks[source].back();
//            stacks[source].pop_back();
//            stacks[dest].push_back(crate);
            // 9001 - Part Two
            crane.push_back(stacks[source].back());
            stacks[source].pop_back();
        }
        for (int i = 0; i < count; i++) {
            stacks[dest].push_back(crane.back());
            crane.pop_back();
        }
        printf("%s\n", printStacks(stacks).c_str());
    }

    printf("Tops: %s", result.c_str());
    return 0;
}
