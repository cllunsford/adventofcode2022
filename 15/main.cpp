#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <set>
#include <sstream>

using namespace std;

vector<pair<int,int>> sensors;
set<pair<int,int>> beacons;
vector<int> distances;

bool isBeacon(int x, int y) {
    for (auto &beacon: beacons) {
        if (beacon.first == x && beacon.second == y) {
            return true;
        }
    }
    return false;
}

int beaconsInRange(int y, int minX, int maxX) {
    int count = 0;
    for (auto &beacon : beacons) {
        if (beacon.first >= minX && beacon.first <= maxX && beacon.second == y) {
            count++;
        }
    }
    return count;
}

int sumRowNearSensors(int y, int minX, int maxX, bool debug) {
    int sum = 0;
    for (int x=minX; x <= maxX; x++) {
        if (isBeacon(x, y)) {
            continue;
        }
        bool found = false;
        for (int i = 0; i < sensors.size(); i++) {
            int dx = abs(sensors[i].first - x);
            int dy = abs(sensors[i].second - y);
            if (dx + dy <= distances[i]) {
                found = true;
                // We can land inside another zone. So we add to sensor X and then the full X distance on the other side
                int span = dx + (distances[i] - dy);
                sum += 1 + span; // Add the span plus the one we're on now
                sum -= beaconsInRange(y, x, x + span);
//                printf("[%d,%d] found Sensor %d [%d, %d] (%d + %d = %d <= %d) sum: %d\n", x, y, i,
//                       sensors[i].first, sensors[i].second, dx, dy, (dx + dy), distances[i], sum);
                x += span;
//                printf(" Skip to %d\n", x);
                break;
            }
        }
        if (!found && debug) {
            cout << "Gap at " << x << "," << y << endl;
            cout << ((long)x * (long)4000000 + (long)y) << endl;
        }
    }
    return sum;
}

int main() {
    cout << "Advent of Code: Day 15" << endl;

    ifstream infile("../day15.txt");

    // Initialize the grid
    string input;
    set<string> scans;

    while (getline(infile, input)) {
        stringstream ss(input);
        string s,at,sx,sy,c,b,i,a,bx,by;
        ss >> s >> at >> sx >> sy >> c >> b >> i >> a >> bx >> by;
        pair<int,int> sensor;
        sensor.first = stoi(sx.substr(2,sx.length() - 3));
        sensor.second = stoi(sy.substr(2,sy.length() - 3));
        sensors.push_back(sensor);
        cout << "Sensor: " << sensor.first << "," << sensor.second << endl;

        pair<int,int> beacon;
        beacon.first = stoi(bx.substr(2,bx.length() - 3));
        beacon.second = stoi(by.substr(2,by.length() - 2));
        beacons.insert(beacon);
        cout << "Beacon: " << beacon.first << "," << beacon.second << endl;

        int distance = abs(sensor.first - beacon.first) + abs(sensor.second - beacon.second);
        cout << "Distance: " << distance << endl << endl;
        distances.push_back(distance);
    }

    // Part One
    int MIN_X = -2000000;
    int MAX_X = 6000000;
    int TEST_Y = 2000000;
    int sum = sumRowNearSensors(TEST_Y, MIN_X, MAX_X, false);

    cout << "Row 2000000: " << sum << endl;

    // Part two
    int MIN = 0;
    int MAX = 4000000;
    for (int y=MIN; y <= MAX; y++) {
        if (sumRowNearSensors(y, MIN, MAX, true) < MAX) {
            cout << "Row with empty loc: " << y << endl;
            break;
        }
    }
    return 0;
}
