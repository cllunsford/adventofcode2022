#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

long yelling(vector<tuple<string,string,string,string>> jobs, map<string,long> monkeys, string term) {
    while (!monkeys.contains(term)) {
        cout << "Jobs remaining: " << jobs.size() << endl;
        for (int i=0; i < jobs.size(); i++) {
            auto job = jobs[i];
            if (monkeys.contains(get<1>(job)) && monkeys.contains(get<3>(job))) {
                long a = monkeys[get<1>(job)];
                long b = monkeys[get<3>(job)];
                string op = get<2>(job);
                if (op == "+") {
                    monkeys[get<0>(job)] = a + b;
                    jobs.erase(jobs.begin() + i);
                } else if (op == "-") {
                    monkeys[get<0>(job)] = a - b;
                    jobs.erase(jobs.begin() + i);
                } else if (op == "*") {
                    monkeys[get<0>(job)] = a * b;
                    jobs.erase(jobs.begin() + i);
                    cout << a << "*" << b << "=" << monkeys[get<0>(job)] << endl;
                } else if (op == "/") {
                    monkeys[get<0>(job)] = a / b;
                    jobs.erase(jobs.begin() + i);
                    cout << a << "/" << b << "=" << monkeys[get<0>(job)] << endl;
                }
            }
        }
    }
    return monkeys[term];
}

long ladder(vector<tuple<string,string,string,string>> jobs, map<string,long> monkeys, string term) {
    bool down = false;
    int remaining = 0;
    int same = 0;
    while (!monkeys.contains(term) && same < 5) {
        cout << "Jobs remaining: " << jobs.size() << endl;
        if (remaining == jobs.size()) {
            same++;
            down = true;
        }
        remaining = jobs.size();
        for (int i=0; i < jobs.size(); i++) {
            auto job = jobs[i];
            string name = get<0>(job);
            string op = get<2>(job);

            // If we've found either of the root operands, we can start down
            if (name == "root") {
                if (monkeys.contains(get<1>(job))) {
                    monkeys[get<3>(job)] = monkeys[get<1>(job)];
                    cout << name << ": " << get<1>(job) << "=>" << get<3>(job) << "=" << monkeys[get<3>(job)] << " ******** " << endl;
                    jobs.erase(jobs.begin() + i);
                    continue;
                } else if (monkeys.contains(get<3>(job))) {
                    monkeys[get<1>(job)] = monkeys[get<3>(job)];
                    cout << name << ": " << get<3>(job) << "=>" << get<1>(job) << "=" << monkeys[get<1>(job)] << " ******** " << endl;
                    jobs.erase(jobs.begin() + i);
                    continue;
                }
            }

            if (down) {
                if (monkeys.contains(get<1>(job)) && monkeys.contains(name)) {
                    long a = monkeys[get<1>(job)];
                    long c = monkeys[name];
                    // c = a ? b
                    if (op == "+") {
                        monkeys[get<3>(job)] = c - a;
                        jobs.erase(jobs.begin() + i);
                        cout << get<3>(job) << ": " << c << "-" << a << "=" << monkeys[get<3>(job)] << endl;
                    } else if (op == "-") {
                        monkeys[get<3>(job)] = a - c;
                        jobs.erase(jobs.begin() + i);
                        cout << get<3>(job) << ": " << a << "-" << c << "=" << monkeys[get<3>(job)] << endl;
                    } else if (op == "*") {
                        monkeys[get<3>(job)] = c / a;
                        jobs.erase(jobs.begin() + i);
                        cout << get<3>(job) << ": " << c << "/" << a << "=" << monkeys[get<3>(job)] << endl;
                    } else if (op == "/") {
                        monkeys[get<3>(job)] = a / c;
                        jobs.erase(jobs.begin() + i);
                        cout << get<3>(job) << ": " << a << "/" << c << "=" << monkeys[get<3>(job)] << endl;
                    }
                } else if (monkeys.contains(get<3>(job)) && monkeys.contains(name)) {
                    long b = monkeys[get<3>(job)];
                    long c = monkeys[name];
                    // c = a ? b
                    if (op == "+") {
                        monkeys[get<1>(job)] = c - b;
                        jobs.erase(jobs.begin() + i);
                        cout << get<1>(job) << ": " << c << "-" << b << "=" << monkeys[get<1>(job)] << endl;
                    } else if (op == "-") {
                        monkeys[get<1>(job)] = c + b;
                        jobs.erase(jobs.begin() + i);
                        cout << get<1>(job) << ": " << c << "+" << b << "=" << monkeys[get<1>(job)] << endl;
                    } else if (op == "*") {
                        monkeys[get<1>(job)] = c / b;
                        jobs.erase(jobs.begin() + i);
                        cout << get<1>(job) << ": " << c << "/" << b << "=" << monkeys[get<1>(job)] << endl;
                    } else if (op == "/") {
                        monkeys[get<1>(job)] = c * b;
                        jobs.erase(jobs.begin() + i);
                        cout << get<1>(job) << ": " << c << "*" << b << "=" << monkeys[get<1>(job)] << endl;
                    }
                }
            } else if (monkeys.contains(get<1>(job)) && monkeys.contains(get<3>(job))) {
                long a = monkeys[get<1>(job)];
                long b = monkeys[get<3>(job)];
                if (op == "+") {
                    monkeys[name] = a + b;
                    jobs.erase(jobs.begin() + i);
                    cout << name << ": " << a << "+" << b << "=" << monkeys[name] << endl;
                } else if (op == "-") {
                    monkeys[name] = a - b;
                    jobs.erase(jobs.begin() + i);
                    cout << name << ": " << a << "-" << b << "=" << monkeys[name] << endl;
                } else if (op == "*") {
                    monkeys[name] = a * b;
                    jobs.erase(jobs.begin() + i);
                    cout << name << ": " << a << "*" << b << "=" << monkeys[name] << endl;
                } else if (op == "/") {
                    monkeys[name] = a / b;
                    jobs.erase(jobs.begin() + i);
                    cout << name << ": " << a << "/" << b << "=" << monkeys[name] << endl;
                }
            }
        }
    }
    return monkeys[term];
}

int main() {
    cout << "Advent of Code: Day 21" << endl;

    ifstream infile("../21/day21.txt");

    map<string,long> monkeys;
    vector<tuple<string,string,string,string>> jobs;
    string input;
    while (getline(infile, input)) {
        string name = input.substr(0, 4);
        if (input.size() == 17) {
            string a = input.substr(6, 4);
            string op = input.substr(11, 1);
            string b = input.substr(13, 4);
            jobs.emplace_back(name, a, op, b);
        } else {
            int value = stoi(input.substr(6, input.size() - 6));
            monkeys[name] = value;
        }
    }

    long root = yelling(jobs, monkeys, "root");
    cout << endl << "Part one: " << root << endl;

    // Part two
    monkeys.erase("humn");
    long humn = ladder(jobs, monkeys, "humn");
    cout << endl << "Part two: " <<  humn << endl;
    return 0;
}