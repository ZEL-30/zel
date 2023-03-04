#include <parser.h>
#include <value.h>
using namespace script::interpreter;

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;


TEST_CASE("文件 测试 Interpreter 类") {

    ifstream fin;
    // fin.open("../script/Authenticate212.txt");
    fin.open("../script/test.txt");
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
