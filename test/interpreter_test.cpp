#include <parser.h>
using namespace zel::interpreter;

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

// TEST_CASE("字符串 测试 Lexer 类") {

//     string source = R"(
//         KEY = 404142434445464748494A4B4C4D4E4F
//         abc = 345LKSDJV3r4

//     )";

//     Parser parser(source);
//     auto v_astnodes = parser.parse();

//     for (int i = 0; i < v_astnodes.size(); i++) {
//         cout << v_astnodes[i]->str() << endl;
//     }

// }

TEST_CASE("文件 测试 Lexer 类") {

    ifstream fin;
    fin.open("../script/Authenticate212.txt");
    // fin.open("../script/test.txt");
    // fin.open("/Users/zel/Workspaces/C++/zel/script/test.txt");
    if (fin.fail()) {
        throw std::logic_error("open script failed.");
    }

    stringstream ss;
    ss << fin.rdbuf();
    string source = ss.str();

    Parser parser(source);
    auto v_ast_nodes = parser.parse();

    for (int i = 0; i < v_ast_nodes.size(); i++) {
        cout << v_ast_nodes[i]->str() << endl;
    }

}
