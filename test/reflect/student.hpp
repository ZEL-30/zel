#pragma once

#include "class_register.hpp"

#include <iostream>

class Student : public zel::reflect::Object {

  public:
    Student() {}
    ~Student() {}

  private:
    std::string name_;
};

REGISTER_CLASS(Student);