#include "teacher.hpp"

#include <iostream>
using namespace std;

#include "class_factory.h"
using namespace zel::reflect;

int main() {

    ClassFactory* factory = zel::utility::Singleton<ClassFactory>::instance();

    Teacher* teacher = (Teacher*)factory->create_class("Teacher");
    teacher->show();

    return 0;
}