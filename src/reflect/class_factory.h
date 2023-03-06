#pragma once

#include "class_field.hpp"
#include "class_method.hpp"
#include "../utility/singleton.hpp"

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace zel {

namespace reflect {

class Object {

  public:
    Object();
    virtual ~Object();

    void className(const std::string& class_name);
    const std::string& className() const;

    int getFieldCount();
    std::shared_ptr<ClassField> getField(int pos);
    std::shared_ptr<ClassField> getField(const std::string& field_name);

    template <typename T>
    void get(const std::string& field_name, T& value);

    template <typename T>
    void set(const std::string& field_name, const T& value);

    void call(const std::string& method_name);
    int call(const std::string& method_name, int a, int b);

  private:
    std::string class_name_;
};

typedef Object* (*create_object)(void);

class ClassFactory {

    friend class utility::Singleton<ClassFactory>;

  public:
    void registerClass(const std::string& class_name, create_object mothod);
    Object* createClass(const std::string& class_name);

    void registerClassField(const std::string& class_name,
                            const std::string& field_name,
                            const std::string& field_type,
                            size_t offset);
    int getFieldCount(const std::string& class_name);
    std::shared_ptr<ClassField> getField(const std::string& class_name, int pos);
    std::shared_ptr<ClassField> getField(const std::string& class_name,
                                         const std::string& field_name);

    void registerClassMethod(const std::string& class_name,
                             const std::string& method_name,
                             uintptr_t method);
    int getMethodCount(const std::string& class_name);
    std::shared_ptr<ClassMethod> getMethod(const std::string& class_name, int pos);
    std::shared_ptr<ClassMethod> getMethod(const std::string& class_name,
                                           const std::string& method_name);

  private:
    ClassFactory();
    ~ClassFactory();

  private:
    std::map<std::string, create_object> m_class_; // 类的名称和创建类的函数的映射

    std::map<std::string, std::vector<std::shared_ptr<ClassField>>>
        m_class_fields_; // 类名, 字段数组

    std::map<std::string, std::vector<std::shared_ptr<ClassMethod>>>
        m_class_methods_; // 类名, 字段数组
};

template <typename T>
void Object::get(const std::string& field_name, T& value) {
    ClassFactory* factory = utility::Singleton<ClassFactory>::instance();
    std::shared_ptr<ClassField> field = factory->getField(class_name_, field_name);
    if (field == nullptr) {
        printf("Class objects '%s' don't have '%s' member attribute\n",
               class_name_.c_str(),
               field_name.c_str());
        return;
    }

    size_t offset = field->offset();
    value = *(T*)((unsigned char*)(this) + offset);
}

template <typename T>
void Object::set(const std::string& field_name, const T& value) {
    ClassFactory* factory = utility::Singleton<ClassFactory>::instance();
    std::shared_ptr<ClassField> field = factory->getField(class_name_, field_name);
    if (field == nullptr) {
        printf("Class objects '%s' don't have '%s' member attribute\n",
               class_name_.c_str(),
               field_name.c_str());
        return;
    }

    size_t offset = field->offset();
    *(T*)((unsigned char*)(this) + offset) = value;
}

} // namespace reflect

} // namespace zel