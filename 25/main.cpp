#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cmath>

using namespace std;

long snafu_to_decimal(string snafu) {
    long dec = 0;
    for (long i=0; i < snafu.size(); i++) {
        long op;
        if (snafu[i] == '=') {
            op = -2;
        } else if (snafu[i] == '-') {
            op = -1;
        } else {
            op = snafu[i] - '0';
        }
        long p = pow(5, ((long)snafu.size() - i - 1));
//        cout << dec << " + " << op << " * " << p << endl;
        dec += op * p;
    }
    return dec;
}

string decimal_to_snafu(long num) {
    string snafu;
    long units = 1;
    // Find how many digits are in the snafu
    while (num > (pow(5,units) - 1) / 2) {
        units++;
    }
//    cout << num << " is " << units << " digit(s)" << endl;
    // For each digit...
    for (long i=units - 1; i >= 0; i--) {
        for (long j=-2; j <= 2; j++) {
            long p = (long) pow(5,i);
            long edge = (p - 1) / 2;

            long left = (j * p) - edge;
            long right = (j * p) + edge;

//            printf("left (%d * %d + %d) = %d\n", j, p, edge, left);
//            printf("right (%d * %d + %d) = %d\n", j + 1, p, edge, right);
//            printf("%d <= %d < %d\n", left, num, right);
            if (num == left || (num >= left && num <= right)) {
                if (j == -2) {
                    snafu += "=";
                } else if (j == -1) {
                    snafu += "-";
                } else {
                    snafu += to_string(j);
                }
                num -= (j * p);
                break;
            }
        }
    }
    return snafu;
}

int main() {
    cout << "Advent of Code: Day 25" << endl;

    ifstream infile("../25/day25.txt");

    vector<string> numbers;
    string input;
    long sum;
    while (getline(infile, input)) {
        numbers.push_back(input);
        long d = snafu_to_decimal(input);
        sum += d;
        string s = decimal_to_snafu(d);
//        cout << input << endl;
        cout << input << " : " << d << " : " << s << endl;

    }
    cout << "Sum: " << sum << " " << decimal_to_snafu(sum) << endl;

    return 0;
}
