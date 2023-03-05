#include "student.hpp"
#include "teacher.hpp"

#include <iostream>
using namespace std;

#include "class_factory.h"
using namespace zel::reflect;

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>



TEST_CASE("反射类") {
    ClassFactory* factory = zel::utility::Singleton<ClassFactory>::instance();

    // string class_name;

    Object* teacher = factory->create_class("Teacher");
    teacher->show();

    Object* student = factory->create_class("Student");
    student->show();
}