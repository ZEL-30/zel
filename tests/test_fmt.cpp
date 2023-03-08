#include <algorithm>
#include <fmt/ranges.h>
#include <iostream>
#include <vector>


using namespace std;

int main() {

    vector<int> v = {1, 3, 4, 5, 6, 7, 8, 423, 4, 234, 123, 2345, 23, 4, 4536, 23, 4512, 123};

    sort(v.begin(), v.end());

    cout << fmt::format("{}", v) << endl;

    v = {1, 2, 3, 4, 5, 56, 6, 7, 8};
    cout << fmt::format("{}", v) << endl;

    return 0;
}