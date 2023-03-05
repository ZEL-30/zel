#pragma once

#include "class_register.hpp"

#include <iostream>

class Teacher : public zel::reflect::Object {

  public:
    Teacher() {}
    ~Teacher() {}

    void show() { std::cout << "This is a teacher class!" << std::endl; }

  private:
    std::string name_;
};

REGISTER_CLASS(Teacher);