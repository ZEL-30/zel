#include "xml/xml.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <iostream>

using namespace zel::xml;
using namespace std;

TEST_CASE("testing Class Xml") {

    Xml s1;
    s1.name("student");
    s1.attr("name", "curry");
    s1.attr("id", 1);
    s1.attr("hight", 1.75);
    s1.text("勇士总冠军");

    Xml s2;
    s2.name("student");
    s2.attr("name", "curry");
    s2.text("勇士总冠军");

    Xml root;
    root.name("students");
    root[0] = s1;
    root.append(s2);

    // cout << root.str() << endl;
    printf("%s\n", root.c_str());

    // // 遍历子节点方式一
    // for (auto it = root.begin(); it != root.end(); it++) {
    //     cout << it->Str() << endl;
    // }

    // // 遍历子节点方式二
    // for (int i = 0; i < root.size(); i++) {
    //     cout << root[i].str() << endl;
    // }


}


TEST_CASE("Xml Parser") {


    Xml root1;
    root1.load("../config/workflow.xml");

    int id = root1[0].attr("name");
    cout << root1[0].name() << id << endl;

    root1.save("../config/temp.xml");


}