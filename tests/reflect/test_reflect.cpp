#include "student.hpp"
#include "string.hpp"

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

    // ClassFactory* factory = zel::utility::Singleton<ClassFactory>::instance();

    // auto teacher = factory->createClass("Teacher");

    // teacher->set("age_", 32);

    // int age = 0;
    // teacher->get("age_", age);
    // cout << "age: " << age << endl;
}

TEST_CASE("类成员函数反射") {

    auto factory = zel::utility::Singleton<ClassFactory>::instance();
    auto class_string = factory->createClass("String");
    auto class_student = factory->createClass("Student");

    int a = 9, b = 30, c = 0;


    string str_return = {0};
    vector<string> v_params = {"404142434445464748494A4B4C4D4E4F", "3", "12"};
    class_string->call("mid", str_return, v_params);



    class_student->call("add", a, b, c);

    cout << "str_return: " << str_return << " c = " << c << endl;
}
