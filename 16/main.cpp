#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <set>
#include <sstream>
#include <map>

using namespace std;

struct valve{
    string label;
    int rate;
    bool open;
    vector<string> exits;
};
map<string, valve> valves;
vector<string> pressuredValves;

typedef pair<string, string> keypair;
map<keypair,int> atlas;

string vectorString(vector<string> valveList) {
    if (valveList.empty()) {
        return "[]";
    }
    string out = valveList[0];
    for (int i = 1; i < valveList.size(); i++) {
        out += "->" + valveList[i];
    }
    return out;
}

string setString(set<string> valveSet) {
    if (valveSet.empty()) {
        return "()";
    }
    string out;
    for (auto & s : valveSet) {
        out += s;
    }
    return out;
}

int findShortestPath(const string& pos, const string& dest, int distance, vector<string> visited) {
    valve current = valves[pos];
    if (current.label == dest) {
        return distance;
    }
    visited.push_back(pos);
    distance++;
    int lowDistance = 999;
    for (auto & exit : current.exits) {
        if (count(visited.begin(), visited.end(), exit)) {
            continue;
        }
        int out = findShortestPath(exit, dest, distance, vector(visited));
        if (out < lowDistance) {
            lowDistance = out;
        }
    }
    return lowDistance;
}

void generateAtlas() {
    vector<string> valveList = vector(pressuredValves);
    valveList.emplace_back("AA");

    for (auto & source : valveList) {
        for (auto & dest : valveList) {
            if (!atlas.contains(keypair(source,dest))) {
                int distance = findShortestPath(source, dest, 0, vector<string>());
                atlas[keypair(source,dest)] = distance;
                atlas[keypair(dest,source)] = distance;
//                printf("%s->%s %d\n", source.c_str(), dest.c_str(), distance);
            }
        }
    }
}

// From starting position, create a possibility tree where each branch is the decision to open a valve
// Each branch ends in a leaf when the last valve is opened
pair<vector<string>,int> findHighestPressure(const string& pos, int pressure, int t, vector<string> opened, const vector<string>& pressured) {
    valve current = valves[pos];
//    cout << vectorString(opened) << " " << pos << " " << pressure << " " << t << endl;

    // If we're on a valved node, open it and calculate the expected total pressure for this node
    if (current.rate > 0) {
        int ep = (t - 1) * current.rate;
        opened.push_back(pos);
        pressure += ep;
        t -= 1; // Spend a turn opening the valve
    }
    // If we reached the last open valve, exit early
    if (opened.size() == pressured.size()) {
//        printf("Candidate %s [%d]\n", vectorString(opened).c_str(), pressure);
        return { opened, pressure };
    }
    // If we ran out of time, exit early. There's no time to move to another node and release pressure
    if (t <= 1) {
//        printf("Ran out of time %s [%d]\n", vectorString(opened).c_str(), pressure);
        return { opened, pressure };
    }

    // Compare all possible additions to our current path
    pair<vector<string>, int> result(opened, pressure);
    for (auto & next : pressured) {
        if (count(opened.begin(), opened.end(), next)) {
            continue;
        }
        // calculate travel time to the new node
        int step = atlas[keypair(pos,next)];
        if (step >= t) {
            continue;
        }
        auto out = findHighestPressure(next, pressure, (t - step), vector(opened), pressured);
        if (out.second > result.second) {
            result = out;
        }
    }
    return result;
}

set<set<string>> generateCombinations(const set<string>& path, vector<string> source) {
    cout << string(path.size(), '#') << " " << setString(path) << " <- " << vectorString(source) << endl;
    set<set<string>> combos;
    for (int i = 0; i < source.size(); i++) {
        set newPath = set(path);
        newPath.insert(source[i]);
        combos.insert(newPath);

        if (newPath.size() < (pressuredValves.size() / 2)) {
            // Key insight here is that the previous paths will handle scenarios before i, so we don't need to repeat them
            auto newVector = vector(source.begin() + i + 1, source.end());
            combos.merge(generateCombinations(newPath, newVector));
        }
    }
    return combos;
}

set<string> invertSet(set<string> source) {
    set<string> out;
    for (auto & v : pressuredValves) {
        if (!source.contains(v)) {
            out.insert(v);
        }
    }
    return out;
}

int main() {
    cout << "Advent of Code: Day 16" << endl;

    ifstream infile("../16/day16.txt");

    string input;
    set<string> scans;

    while (getline(infile, input)) {
        string current = input.substr(0, input.find(';'));
        string paths = input.substr(input.find(';'));

        stringstream ss(current);
        string v,label,h,f,r;
        ss >> v >> label >> h >> f >> r;
        int rate = stoi(r.substr(r.find('=') + 1, r.length() - 5));

        stringstream ps(paths);
        string tmp;
        vector<string> exits;
        while(getline(ps, tmp, ',')) {
            string exit = tmp.substr(tmp.length()-2, 2);
            exits.push_back(exit);
        }

        if (rate > 0) {
            pressuredValves.push_back(label);
        }

        auto newValve = new valve;
        newValve->label = label;
        newValve->rate = rate;
        newValve->exits = exits;
        newValve->open = false;
        valves[newValve->label] = *newValve;
    }

    cout << "Valves with pressure: " << pressuredValves.size() << endl;
    generateAtlas();

    // Part One
    auto output = findHighestPressure("AA", 0, 30, vector<string>(), pressuredValves);
    cout << "Best Path: " << vectorString(output.first) << endl;
    cout << "Total Pressure released: " << output.second << endl;

    // Part Two - find the highest disjoint pair of paths
    // This method still takes about 10 minutes to run. Probably another optimization to be made here
    // Maybe we can assume that the two halves are roughly equal and start with the paths more evenly split?
    auto sets = generateCombinations(set<string>(), pressuredValves);
    cout << "Path combinations: " << sets.size() << endl;
    int highestCombined = 0;
    int MIN_SET_SIZE = 5;
    for (auto & s : sets) {
        // Just to save some time, we might assume that the optimal solutions are more balanced. So skip running
        // the pairs where one side is much longer than the other. About 5 minutes with MSS = 5
        if (s.size() < MIN_SET_SIZE) {
            continue;
        }
        set si = invertSet(s);
        auto sOut = findHighestPressure("AA", 0, 26, vector<string>(), vector(s.begin(), s.end()));
        auto siOut = findHighestPressure("AA", 0, 26, vector<string>(), vector(si.begin(), si.end()));
        int combined = (sOut.second + siOut.second);
        cout << "Pressure for set " << setString(s) << " = " << combined << endl;
        if (combined > highestCombined) {
            highestCombined = combined;
        }
    }
//    cout << "Best Path: " << vectorString(output.first) << endl;
    cout << "Total Pressure released: " << highestCombined << endl;

    return 0;
}