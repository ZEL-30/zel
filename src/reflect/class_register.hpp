#pragma once

#include "class_factory.h"

#include <string>

namespace zel {

namespace reflect {

class ClassRegister {

  public:
    ClassRegister(const std::string& class_name, create_object method) {
        printf("class register %s\n", class_name.c_str());
        ClassFactory* factory = utility::Singleton<ClassFactory>::instance();
        factory->registerClass(class_name, method);
    }
    ClassRegister(const std::string& class_name,
                  const std::string& field_name,
                  const std::string& field_type,
                  size_t offset) {
        printf("class register class: %s, field: %s\n", class_name.c_str(), field_name.c_str());
        ClassFactory* factory = utility::Singleton<ClassFactory>::instance();
        factory->registerClassField(class_name, field_name, field_type, offset);
    }
    ClassRegister(const std::string& class_name, const std::string& method_name, uintptr_t method) {
        printf("class register class: %s, method: %s\n", class_name.c_str(), method_name.c_str());
        ClassFactory* factory = utility::Singleton<ClassFactory>::instance();
        factory->registerClassMethod(class_name, method_name, method);
    }

    ~ClassRegister() {}

  private:
};

#define REGISTER_CLASS(class_name)                                                                 \
    zel::reflect::Object* createObject##class_name() {                                             \
        zel::reflect::Object* obj = new class_name();                                              \
        obj->className(#class_name);                                                               \
        return obj;                                                                                \
    }                                                                                              \
    zel::reflect::ClassRegister classRegister##class_name(#class_name, createObject##class_name)

#define REGISTER_CLASS_FIELD(class_name, field_name, field_type)                                   \
    class_name class_name##field_name;                                                             \
    zel::reflect::ClassRegister classRegister##class_name##field_name(                             \
        #class_name,                                                                               \
        #field_name,                                                                               \
        #field_type,                                                                               \
        (size_t)(&(class_name##field_name.field_name)) - (size_t)(&(class_name##field_name)))

#define REGISTER_CLASS_METHOD(class_name, method_name)                                             \
    std::function<int(class_name*, int, int)> class_name##method_name##method = &class_name::method_name;   \
    zel::reflect::ClassRegister classRegister##class_name##method_name(                            \
        #class_name, #method_name, (uintptr_t) & (class_name##method_name##method))

} // namespace reflect

} // namespace zel
