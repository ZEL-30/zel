#include "xml/xml.h"

#include <iostream>
#include <stdexcept>
using namespace zel::utility;
using namespace std;

int main() {

    Xml s1;
    s1.SetName("student");
    s1.SetAttr("name", "curry");
    s1.SetAttr("id", 1);
    s1.SetAttr("hight", 1.75);
    s1.SetText("勇士总冠军");

    Xml s2;
    s2.SetName("student");
    s2.SetAttr("name", "curry");
    s2.SetText("勇士总冠军");


    Xml root;
    root.SetName("students");
    root[0] = s1;
    root.Append(s2);

    cout << root.AsString() << endl;

    // // 遍历子节点方式一
    // for (auto it = root.begin(); it != root.end(); it++) {
    //     cout << it->AsString() << endl;
    // }

    // 遍历子节点方式二
    for (int i = 0; i < root.size(); i++) {
        cout << root[i].AsString() << endl;
    }

    return 0;
}