#include "xml/xml.h"

#include <iostream>
#include <stdexcept>
using namespace zel::utility;
using namespace std;

int main() {

    // Xml s1;
    // s1.SetName("student");
    // s1.SetAttr("name", "curry");
    // s1.SetAttr("id", 1);
    // s1.SetAttr("hight", 1.75);
    // s1.SetText("勇士总冠军");

    // Xml s2;
    // s2.SetName("student");
    // s2.SetAttr("name", "curry");
    // s2.SetText("勇士总冠军");


    // Xml root;
    // root.SetName("students");
    // root[0] = s1;
    // root.Append(s2);

    // cout << root.AsString() << endl;

    // // // 遍历子节点方式一
    // // for (auto it = root.begin(); it != root.end(); it++) {
    // //     cout << it->AsString() << endl;
    // // }

    // // 遍历子节点方式二
    // for (int i = 0; i < root.size(); i++) {
    //     cout << root[i].AsString() << endl;
    // }

    Xml root1;
    root1.Load("../config/workflow.xml");

    cout << root1.AsString() << endl;

    // Xml s1;
    // s1.name("studen");
    // s1.attr("team", "warriors");
    // s1.text("勇士总冠军");

    // root1.Append(s1);

    // cout << root1.AsString() << endl;

    // root1.Save("../config/temp.xml");

    return 0;
}