#include <vector>
#include <iostream>
#include <climits>

struct Node {
    int key;
    long left = 0;
    long right = 0;
    char color = 'B';
};

bool Leaf(std::vector<Node>& treeVec, int number) {
    return ((treeVec[number].left == 0) && (treeVec[number].right == 0));
}

bool Binary(std::vector<Node>& treeVec, int number, int min, int max) {
    if (number == 0) {
        return true;
    }
    if ((treeVec[number].key <= min) || (treeVec[number].key >= max)) {
        return false;
    }
    return Binary(treeVec, treeVec[number].left, min, treeVec[number].key) &&
           Binary(treeVec, treeVec[number].right, treeVec[number].key, max);
}

bool NotColorful(std::vector<Node>& treeVec, int number) {
    return ((treeVec[number].color == 'R') && ((treeVec[treeVec[number].left].color == 'R') ||
                                 (treeVec[treeVec[number].right].color == 'R')));
}

std::pair<bool, int> BlackHeight(std::vector<Node>& treeVec, int number) {
    std::pair<bool, int> res, left, right;
    if (number == 0) {
        res.first = true;
        res.second = 0;
        return res;
    }
    left = BlackHeight(treeVec, treeVec[number].left);
    right = BlackHeight(treeVec, treeVec[number].right);
    if (( !(left.first) || !(right.first)) || (left.second != right.second)
                        || NotColorful(treeVec, number)) {
        res.first = false;
        res.second = 0;
        return res;
    }
    res.first = true;
    res.second = right.second;
    if (treeVec[number].color == 'B') {
        ++res.second;
    }
    return res;
}

int main() {
    std::vector<Node> treeVec;
    int n, root;
    std::cin >> n;
    if (!n) {
        std::cout << "YES";
        return 0;
    }
    std::cin >> root;
    treeVec.resize(n + 1);
    Node temp;
    treeVec[0] = temp;
    for (int i = 0; i < n; ++i) {
        Node temp;
        size_t number;
        std::cin >> number >> temp.key;
        std::string s;
        std::cin >> s;
        if (s == "null") {
            temp.left = 0;
        } else {
            temp.left = std::stoi(s);
        }
        std::cin >> s;
        if (s == "null") {
            temp.right = 0;
        } else {
            temp.right = std::stoi(s);
        }
        std::cin >> temp.color;
        treeVec[number] = temp;
    }

    if (treeVec[root].color == 'R') {
        std::cout << "NO\n";
        return 0;
    }
    if (!BlackHeight(treeVec, root).first || !Binary(treeVec, root, INT_MIN, INT_MAX)) {
        std::cout << "NO\n";
        return 0;
    }
    std::cout << "YES\n";
    return 0;
}