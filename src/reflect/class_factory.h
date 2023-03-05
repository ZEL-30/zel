#pragma once

#include "singleton.hpp"

#include <map>
#include <string>

namespace zel {

namespace reflect {

class Object {

  public:
    Object(); 
    virtual ~Object();

    virtual void show() = 0;
};

typedef Object* (*create_object)(void);

class ClassFactory {

    friend class utility::Singleton<ClassFactory>;

  public:
    void register_class(const std::string& class_name, create_object mothod);
    Object* create_class(const std::string& class_name);

  private:
    ClassFactory();
    ~ClassFactory();

  private:
    std::map<std::string, create_object> m_class_; // 类的名称和创建类的函数的映射
};

} // namespace reflect

} // namespace zel