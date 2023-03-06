#include "student.hpp"
#include "teacher.hpp"

#include <iostream>
using namespace std;

#include "class_factory.h"
using namespace zel::reflect;

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("类对象反射") {
    ClassFactory* factory = zel::utility::Singleton<ClassFactory>::instance();

    // string class_name;

    Object* teacher = factory->createClass("Teacher");

    Object* student = factory->createClass("Student");
}

TEST_CASE("类成员数据反射") {

    ClassFactory* factory = zel::utility::Singleton<ClassFactory>::instance();

    auto teacher = factory->createClass("Teacher");

    teacher->set("age", 32);

    int age = 0;
    teacher->get("age", age);
    cout << "age: " << age << endl;
}

TEST_CASE("类成员函数反射") {

    auto factory = zel::utility::Singleton<ClassFactory>::instance();
    auto teacher = factory->createClass("Teacher");

    auto c = teacher->call("add", 1, 4);

    cout << c << endl;
}
