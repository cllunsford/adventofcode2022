#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <regex>
#include <algorithm>

using namespace std;

pair<string,int> walkBuilds(const vector<int>& bp, int t, int ore_max, int ore_rob, int clay_rob, int obs_rob, int geo_rob,
                            int ore, int clay, int obs, int geo, const string& path) {
    // Reached the end of the allowed time, just generate the last geodes and return
    if (t == 1) {
//        cout << t << ": " << path + "-" << " " << geo + geo_rob << endl;
        return { path + "-", geo + geo_rob };
    }

    pair<string,int> highest(path, geo);
    // If we can build a geode, always do it
    if (ore >= bp[4] && obs >= bp[5]) {
        auto result = walkBuilds(bp, (t-1), ore_max, ore_rob, clay_rob, obs_rob, geo_rob + 1,
                                 ore - bp[4] + ore_rob, clay + clay_rob, obs - bp[5] + obs_rob, geo + geo_rob, path + "G");
        if (result.second > highest.second) {
            highest = result;
        }
    } else {
        // Optimization: if we don't have a geode robot by this point in turn = 2, we're not going to get any geodes
        if (t <= 2 && geo_rob == 0) {
            return highest;
        }

        // Obsidian resources. Don't build more robots than the consuming robot requires (can't build more than 1 robot per turn)
        if (obs_rob < bp[5] && ore >= bp[2] && clay >= bp[3]) {
            // Don't build obs if we could have built it last turn and waited instead
            if (!(path.back() == '.' && bp[2] <= ore - ore_rob && bp[3] <= clay - clay_rob)) {
                auto result = walkBuilds(bp, (t - 1), ore_max, ore_rob, clay_rob, obs_rob + 1, geo_rob,
                                         ore - bp[2] + ore_rob, clay - bp[3] + clay_rob, obs + obs_rob, geo + geo_rob,
                                         path + "O");
                if (result.second > highest.second) {
                    highest = result;
                }
            }
        }

        // Optimization: if we don't have an obs robot by this point in turn = 3, we're not going to get any geodes
        if (t <= 3 && obs_rob == 0) {
            return highest;
        }

        // Clay
        if (clay_rob < bp[3] && ore >= bp[1]) {
            // Don't build clay if we could have built it last turn and waited instead
            if (!(path.back() == '.' && bp[1] <= ore - ore_rob)) {
                auto result = walkBuilds(bp, (t - 1), ore_max, ore_rob, clay_rob + 1, obs_rob, geo_rob,
                                         ore - bp[1] + ore_rob, clay + clay_rob, obs + obs_rob, geo + geo_rob,
                                         path + "C");
                if (result.second > highest.second) {
                    highest = result;
                }
            }
        }

        // Ore
        if (ore_rob < ore_max && ore >= bp[0]) {
            // Don't build ore if we could have built it last turn and waited instead
            if (!(path.back() == '.' && bp[0] <= ore - ore_rob)) {
                auto result = walkBuilds(bp, (t-1), ore_max, ore_rob + 1, clay_rob, obs_rob, geo_rob,
                                         ore - bp[0] + ore_rob, clay + clay_rob, obs + obs_rob, geo + geo_rob, path + "R");
                if (result.second > highest.second) {
                    highest = result;
                }
            }
        }

        // Wait
        // Don't wait if we're sitting on double ore
        if (ore < ore_max * 2) {
            auto result = walkBuilds(bp, (t-1), ore_max, ore_rob, clay_rob, obs_rob, geo_rob,
                                     ore + ore_rob, clay + clay_rob, obs + obs_rob, geo + geo_rob, path + ".");
            if (result.second > highest.second) {
                highest = result;
            }
        }
    }
    return highest;
}

int main() {
    cout << "Advent of Code: Day 19" << endl;

    ifstream infile("../19/day19.txt");

    string input;
    const regex res_regex(R"(Blueprint (\d+): Each ore robot costs (\d+) ore. Each clay robot costs (\d+) ore. Each obsidian robot costs (\d+) ore and (\d+) clay. Each geode robot costs (\d+) ore and (\d+) obsidian.)");
    vector<vector<int>> blueprints;
    while (getline(infile, input)) {
        smatch costs;
        if (regex_search(input, costs, res_regex)) {
            blueprints.push_back({
                                         stoi(costs[2]),
                                         stoi(costs[3]),
                                         stoi(costs[4]),
                                         stoi(costs[5]),
                                         stoi(costs[6]),
                                         stoi(costs[7])
                                 });
        }
    }

    cout << "Blueprints: " << blueprints.size() << endl;

    // Part One
    int TIME_LIMIT = 24;
    int totalQuality = 0;
    for (int i=0; i < blueprints.size(); i++) {
        auto bp = blueprints[i];
        cout << "Blueprint " << i + 1 << endl;
        //int quality = runBlueprint(blueprints[i], TIME_LIMIT);
        printf("R: %d, C: %d, O: %d+%d, G: %d+%d\n", bp[0], bp[1], bp[2], bp[3], bp[4], bp[5]);
        int ore_max = max(bp[0], max(bp[1], max(bp[2], bp[4])));
        auto result = walkBuilds(blueprints[i],TIME_LIMIT,ore_max,1,0,0,0,0,0,0,0,"");
        int quality = result.second;
        cout << "Quality level: " << quality << " " << result.first << endl << endl;
        totalQuality += (i + 1) * quality;
    }
    cout << "Part one: " << totalQuality << endl;

    // Part Two
    int TWO_TIME_LIMIT = 32;
    int part_two_quality = 1;
    for (int i=0; i < 3; i++) {
        auto bp = blueprints[i];
        cout << "Blueprint " << i + 1 << endl;
        printf("R: %d, C: %d, O: %d+%d, G: %d+%d\n", bp[0], bp[1], bp[2], bp[3], bp[4], bp[5]);
        int ore_max = max(bp[0], max(bp[1], max(bp[2], bp[4])));
        auto result = walkBuilds(blueprints[i],TWO_TIME_LIMIT,ore_max,1,0,0,0,0,0,0,0,"");
        int quality = result.second;
        cout << "Quality level: " << quality << " " << result.first << endl << endl;
        part_two_quality *= quality;
    }
    cout << "Part two: " << part_two_quality << endl;
    return 0;
}