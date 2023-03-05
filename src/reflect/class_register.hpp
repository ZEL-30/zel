#pragma once

#include "class_factory.h"

#include <string>

namespace zel {

namespace reflect {

class ClassRegister {

  public:
    ClassRegister(const std::string& class_name, create_object method) {
        ClassFactory* factory = utility::Singleton<ClassFactory>::instance();
        factory->register_class(class_name, method);
    }
    ~ClassRegister() {}

  private:
};

#define REGISTER_CLASS(class_name)                                                                 \
    zel::reflect::Object* createObject##class_name() {                                                           \
        zel::reflect::Object* obj = new class_name();                                                            \
        return obj;                                                                                \
    }                                                                                              \
    zel::reflect::ClassRegister classRegister##class_name(#class_name, createObject##class_name)

} // namespace reflect

} // namespace zel
