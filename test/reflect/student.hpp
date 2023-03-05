#pragma once

#include "class_register.hpp"

#include <iostream>

class Student : public zel::reflect::Object {

  public:
    Student() {}
    ~Student() {}

    void show() { std::cout << "Tiis is a student class!" << std::endl; }

  private:
    std::string name_;
};

REGISTER_CLASS(Student);