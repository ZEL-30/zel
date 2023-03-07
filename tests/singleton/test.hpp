#pragma once

#include <string>
#include <singleton.hpp>

class Test {

    friend zel::utility::Singleton<Test>;

  public:
    void name(const std::string& name) { name_ = name; }
    std::string name() { return name_; }

    void age(int age) { age_ = age; }
    int age() { return age_; }

    void show() {
        printf("name: %s, age: %d\n", name().c_str(), age());
    }

  private:
    Test() {}
    Test(const std::string& name, int age) : name_(name), age_(age) {}
    ~Test() {}

  private:
    std::string name_;
    int age_;
};