#include <cstddef>
#include <iostream>
#include <vector>

using namespace std;

struct st_test {
    string name;
    int age;
    string s_id;
};

int main() {

    // st_test t1;
    
    // t1.name = "张恩乐";
    // t1.age = 25;
    // t1.s_id = "201802464052";

    // auto p = &t1;

    // cout << "name: " << *(string *)(p + offsetof(st_test, name)) << endl;

    std::vector<int> v;

    v = {1,3,35,46,35,56};



    return 0;
}
