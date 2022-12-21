#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <regex>
#include <algorithm>

using namespace std;

string vectorString(vector<int> codes) {
    if (codes.empty()) {
        return "[]";
    }
    string out = to_string(codes[0]);
    for (int i = 1; i < codes.size(); i++) {
        out += "." + to_string(codes[i]);
    }
    return out;
}

string decodeVector(vector<int> values, vector<int> proxy) {
    string out = to_string(values[proxy[0]]);
    for (int i = 1; i < proxy.size(); i++) {
        out += "." + to_string(values[proxy[i]]);
    }
    return out;
}

void run_round(vector<long> &codes, vector<int> &proxy, int size) {
    for (int i=0; i < size; i++) {
        // Find the index of the next elem in the proxy
        int index = (int) distance(proxy.begin(), find(proxy.begin(), proxy.end(), i));
        long elem = codes[i]; // the elem value
        int s = size - 1;
        long dest = ((index + elem) % s + s) % s;

//        cout << elem << " from " << index << " to " << dest << endl;
        proxy.erase(proxy.begin() + index);
        proxy.insert(proxy.begin() + dest, i);
//        cout << decodeVector(codes, proxy) << endl << endl;
    }
}

int main() {
    cout << "Advent of Code: Day 20" << endl;

    ifstream infile("../20/day20.txt");

    vector<long> codes;
    string input;
    while (getline(infile, input)) {
        codes.push_back(stoi(input));
    }

    cout << "Codes: " << codes.size() << endl;
//    cout << vectorString(codes) << endl << endl;

    // Create a surrogate vector with unique indices
    int size = (int) codes.size();
    vector<int> proxy;
    for (int i=0; i < size; i++) {
        proxy.push_back(i);
    }

    // Part One
    auto codes_one = vector(codes);
    auto proxy_one = vector(proxy);
    run_round(codes_one, proxy_one, size);
    long zero = (long) distance(codes_one.begin(), find(codes_one.begin(), codes_one.end(), 0));
    long zero_pos = (long) distance(proxy_one.begin(), find(proxy_one.begin(), proxy_one.end(), zero));
    cout << "Zero pos: " << zero_pos << endl;
    long one = codes_one[proxy_one[(zero_pos + 1000) % size]];
    long two = codes_one[proxy_one[(zero_pos + 2000) % size]];
    long three = codes_one[proxy_one[(zero_pos + 3000) % size]];
    printf("Sum: %ld + %ld + %ld = %ld\n", one, two, three, (one + two + three));

    // Part Two
    for (long & code : codes) {
        code = code * 811589153;
    }
    for (int i=0; i < 10; i++) {
        cout << "Round " << i << endl;
        run_round(codes, proxy, size);
        zero = (long) distance(codes.begin(), find(codes.begin(), codes.end(), 0));
        zero_pos = (long) distance(proxy.begin(), find(proxy.begin(), proxy.end(), zero));
        cout << "Zero pos: " << zero_pos << endl;
    }
    one = codes[proxy[(zero_pos + 1000) % size]];
    two = codes[proxy[(zero_pos + 2000) % size]];
    three = codes[proxy[(zero_pos + 3000) % size]];
    printf("Sum: %ld + %ld + %ld = %ld\n", one, two, three, (one + two + three));
    return 0;
}