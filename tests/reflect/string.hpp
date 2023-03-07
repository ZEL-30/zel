#pragma once

#include "class_register.hpp"

#include <iostream>

namespace scirpt {

namespace function {

class String : public zel::reflect::Object {

  public:
    String() {}
    ~String() {}

    // void f1() { std::cout << "f1" << std::endl; }

    int add(int a, int b) { return a + b; }

    void mid(std::string& str_return, std::vector<std::string> params) {
        int pos = atoi(params.at(1).c_str());
        int len = atoi(params.at(2).c_str());

        str_return = params.at(0).substr(pos, len);
    }

  public:
    std::string name_;
    int age_;
};

REGISTER_CLASS(String);
REGISTER_CLASS_FIELD(String, name_, std::string);
REGISTER_CLASS_FIELD(String, age_, int);
// REGISTER_CLASS_METHOD(String, add, int, int, int);
REGISTER_CLASS_METHOD(String, mid, void, std::string& str_return, std::vector<std::string> params);

} // namespace function

} // namespace scirpt