#pragma once

#include "class_register.h"

#include <iostream>

class Teacher {

  public:
    Teacher() {}
    ~Teacher() {}

    void show() { std::cout << "Teacher" << std::endl; }

  private:
    std::string name_;
};

REGISTER_CLASS(Teacher);