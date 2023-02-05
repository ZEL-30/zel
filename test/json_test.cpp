#include "json/json.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using namespace zel::json;

TEST_CASE("测试Json的基本类型") {

    Json v1;
    Json v2 = true;
    Json v3 = 123;
    Json v4 = 12.3;
    Json v5 = "勇士总冠军";

    cout << v2.str() << endl;
    cout << v5.str() << endl;
    cout << v3.str() << endl;

}

TEST_CASE("测试Json的数组") {

    Json arr;
    arr[0] = true;
    arr[1] = 10;

    arr.Append(1.23);
    arr.Append("zel");
    arr.Append(false);

    int i = arr[1];
    double d = arr[2];

    cout << arr.str() << endl;
    arr.Remove(1);
    cout << arr.str() << endl;
}

// TEST_CASE("测试Json的对象") {

//     Json obj;

//     obj["name"] = "zel";
//     obj["is man"] = true;
//     obj["hight"] = 1.74;
//     obj["sid"] = 1;

//     cout << obj.str() << endl;
//     obj.Remove("hight");
//     cout << obj.str() << endl;

//     obj.Clear();
// }

TEST_CASE("Json Parser") {

    auto source = R"(
        {
            "glossary" : {
                "test": true,
                "hello": null,
                "hello2": "miaomaio",
                "age": -124.4
            }, 
            "data": [1,2,null ,"name"  ,true ,false]
        }
    )";




    Json json;
    json.Parse(source);
    cout << json["data"][3].str() << endl;



    // std::ifstream fin;
    // fin.open("/Users/zel/Workspaces/C++/zel/config/test.json");

    // std::stringstream ss;
    // ss << fin.rdbuf();

    // string source = ss.str();

    // Json json;
    // json.Parse(source);

    // cout << json["data"][9]["tag_name"].str() << endl;

}

