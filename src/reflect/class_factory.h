#pragma once

#include "../utility/singleton.hpp"
#include "class_field.hpp"
#include "class_method.hpp"

#include <functional>
#include <map>
#include <memory>
#include <sstream>
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

    template <typename R = void, typename... Args>
    R call(const std::string& methodName, Args&... args);

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
        std::ostringstream os;
        os << "reflect field " << field_name << " not exists";
        throw std::logic_error(os.str());
    }

    size_t offset = field->offset();
    value = *(T*)((unsigned char*)(this) + offset);
}

template <typename T>
void Object::set(const std::string& field_name, const T& value) {
    ClassFactory* factory = utility::Singleton<ClassFactory>::instance();
    std::shared_ptr<ClassField> field = factory->getField(class_name_, field_name);

    if (field == nullptr) {
        std::ostringstream os;
        os << "reflect field " << field_name << " not exists";
        throw std::logic_error(os.str());
    }

    size_t offset = field->offset();
    *(T*)((unsigned char*)(this) + offset) = value;
}

template <typename R, typename... Args>
R Object::call(const std::string& method_name, Args&... args) {
    ClassFactory* factory = zel::utility::Singleton<ClassFactory>::instance();
    std::shared_ptr<ClassMethod> method = factory->getMethod(class_name_, method_name);
    if (method == nullptr) {
        std::ostringstream os;
        os << "reflect method " << method_name << " not exists";
        throw std::logic_error(os.str());
    }
    auto func = method->method();
    typedef std::function<R(decltype(this), Args & ...)> class_method;
    return (*((class_method*)func))(this, args...);
}

} // namespace reflect

} // namespace zel