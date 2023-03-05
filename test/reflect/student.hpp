#pragma once

#include <iostream>

class Student {

  public:
    Student() {}
    ~Student() {}

    void show() {
        std::cout << "Student" << std::endl;
    }

  private:
    std::string name_;
};