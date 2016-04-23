#include <iostream>
#include "hash_map.h"

using namespace std;

void print(string msg, hash_map &mp) {
    cout << msg << '\n';
    for (auto it = mp.begin(); it != mp.end(); it++) {
        cout << *it << ' ';
    }
    cout << '\n';
}

const string alphabet = "abcdefghijklmnopqrstuvwxyz";

int main() {
    cout << "Hello, World!" << endl;
    hash_map h;
    h.insert({"one", 1});
    h.insert({"two", 2});
    h.insert({"three", 3});
    h.insert({"four", 4});
    h.insert({"five", 5});
    h.insert({"six", 6});
    h.insert({"seven", 7});
    h.insert({"a", 8});
    h.insert({"b", 9});
    h.insert({"c", 10});
    h.insert({"d", 11});
    h.insert({"e", 12});
    print("Initial:", h);
    hash_map copy = h;
    print("Copy:", copy);
    cout << "Erase:\n";
    for (auto i = h.begin(); i != h.end();) {
        cout << (*i) << ' ';
        i = h.erase(i);
    }
    cout << '\n';
    cout << "Backward:\n";
    for (auto i = --copy.end(); i != copy.begin(); --i) {
        cout << *i << ' ';
    }
    cout << '\n';
    print("Erased:", h);
    print("Copy2:", copy);
    hash_map new_map;
    new_map.insert({"wow", 0});
    new_map.insert({"such", -1});
    new_map.insert({"woow", 26});
    new_map.insert({"it's", 13});
    new_map.insert({"working", 4});
    print("Such test:", new_map);
    copy = new_map;
    print("Such test copy", copy);

    size_t n = 500000;
    vector<string> ans(n);
    hash_map large_data;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 25; j++) {
            ans[i] += alphabet[rand() % alphabet.size()];
        }
        large_data.insert({ans[i], i});
    }
    cout << large_data.size() << '\n';
    for (int i = 0; i < n; i++) {
        if (i != *large_data.find(ans[i])) {
            cerr << "Oops\n" << i << ' ' << ans[i];
            return 1;
        }
    }
    return 0;
}