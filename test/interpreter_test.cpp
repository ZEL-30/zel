#include <lexer.h>
using namespace zel::interpreter;

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

TEST_CASE("测试 Lexer 类") {

    // ifstream fin;
    // fin.open("../script/test.txt");
    // if (fin.fail()) {
    //     throw std::logic_error("open script failed.");
    // }

    // stringstream ss;
    // ss << fin.rdbuf();
    // string source = ss.str();

    string source = "==";

    Lexer lexer(source);
    auto v_tokens = lexer.Tokenize(); 

    cout << v_tokens[0].str() << endl;



}
