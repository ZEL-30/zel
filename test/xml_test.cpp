#include "xml/xml.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <iostream>

using namespace zel::xml;
using namespace std;


TEST_CASE("testing Class Xml") {

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

    int id = root1[0].attr("name");
    cout << root1[0].name() << id << endl;

    root1.Save("../config/temp.xml");
}