#pragma once

#include "class_register.hpp"

#include <iostream>

class Student : public zel::reflect::Object {

  public:
    Student() {}
    ~Student() {}

    void add(int a, int b, int& c) { c = a + b; }

  private:
    std::string name_;
};

REGISTER_CLASS(Student);
REGISTER_CLASS_METHOD(Student, add, void, int, int, int&);