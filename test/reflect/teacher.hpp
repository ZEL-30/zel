#pragma once

#include "class_register.hpp"

#include <iostream>

class Teacher : public zel::reflect::Object {

  public:
    Teacher() {}
    ~Teacher() {}

    // void f1() { std::cout << "f1" << std::endl; }

    int add(int a, int b) { 
      int c = a + b;
      return c; 
    }

  public:
    std::string name_;
    int age_;
};

REGISTER_CLASS(Teacher);
REGISTER_CLASS_FIELD(Teacher, name_, std::string);
REGISTER_CLASS_FIELD(Teacher, age_, int);
REGISTER_CLASS_METHOD(Teacher, add);