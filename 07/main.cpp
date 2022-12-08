#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

using namespace std;

struct leaf{
    string name;
    int size;
};

struct tree{
    string name;
    tree *parent;
    int size;
    vector<tree *> dirs;
    vector<leaf *> files;
};

void addDir(tree* &base, string name) {
    tree *newTree;
    newTree = new tree;
    newTree->name = std::move(name);
    newTree->parent = base;
    (base->dirs).push_back(newTree);
}

void addFile(tree* &base, string name, int size) {
    leaf *newLeaf;
    newLeaf = new leaf;
    newLeaf->name = std::move(name);
    newLeaf->size = size;
    (base->files).push_back(newLeaf);
}

int calcTreeSize(tree* &base) {
    if (base->size > 0) {
        return base->size;
    }
    int total = 0;
    for (auto & file : base->files) {
        total += file->size;
    }
    for (auto & dir : base->dirs) {
        total += calcTreeSize(dir);
    }
    base->size = total;
    return total;
}

vector<tree *> findTreesSmallerThan(tree* &base, int cap) {
    vector<tree *> matches;
    if (base->size <= cap) {
        matches.push_back(base);
    }
    for (auto & dir : base->dirs) {
        vector<tree *> dirMatches = findTreesSmallerThan(dir, cap);
        matches.insert(matches.end(), dirMatches.begin(), dirMatches.end());
    }
    return matches;
}

vector<tree *> findTreesBiggerThan(tree* &base, int cap) {
    vector<tree *> matches;
    if (base->size >= cap) {
        matches.push_back(base);
    }
    for (auto & dir : base->dirs) {
        vector<tree *> dirMatches = findTreesBiggerThan(dir, cap);
        matches.insert(matches.end(), dirMatches.begin(), dirMatches.end());
    }
    return matches;
}

int main() {
    cout << "Advent of Code: Day 7" << endl;

    ifstream infile("../day7.txt");

    // Initialize the tree root
    tree *root;
    root = new tree;
    root->name = "/";

    tree *current;
    current = root;

    string input;
    bool listing = false;
    while (getline(infile, input)) {
        stringstream ss(input);
        //cout << input << endl;
        string a,b,c;
        ss >> a >> b >> c;
        if (a == "$") {
            listing = false;
            if (b == "ls") {
                cout << "ls " << endl;
                listing = true;
            } else if (b == "cd") {
                if (c == "/") {
                    // Handle the base case for starting the tree
                    cout << "Already at " << c << endl;
                } else if (c == "..") {
                    // Set current to current parent
                    current = current->parent;
                    cout << "CD .. to " << current->name << endl;
                } else {
                    // CD down into child
                    for (auto & child : current->dirs) {
                        if (c == child->name) {
                            current = child;
                            cout << "CD to " << current->name << endl;
                            break;
                        }
                    }
                }
            }
        } else if (listing) {
            if (a == "dir") {
                cout << "Add directory: " << b << endl;
                addDir(current, b);
            } else {
                cout << "Add file: " << b << endl;
                addFile(current, b, stoi(a));
            }
        }
    }

    int total = calcTreeSize(root);
    cout << "Total: " << total << endl;

    vector<tree *> smallTrees = findTreesSmallerThan(root, 100000);
    int smallTotal = 0;
    for (auto & smallTree : smallTrees) {
        printf("Small Tree: %s %d\n", smallTree->name.c_str(), smallTree->size);
        smallTotal += smallTree->size;
    }
    cout << "Small Total: " << smallTotal << endl;

    int diskCapacity = 70000000;
    int requiredCapacity = 30000000;
    int requiredDeletion = total - (diskCapacity - requiredCapacity);
    cout << "Need to delete: " << requiredDeletion << endl;
    vector<tree *> bigTrees = findTreesBiggerThan(root, requiredDeletion);
    int smallest = total;
    for (auto & bigTree : bigTrees) {
        printf("Big Tree: %s %d\n", bigTree->name.c_str(), bigTree->size);
        if (bigTree->size < smallest) {
            smallest = bigTree->size;
        }
    }
    cout << "Smallest big tree: " << smallest << endl;
    return 0;
}
