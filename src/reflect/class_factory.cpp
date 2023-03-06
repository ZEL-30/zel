#include "class_factory.h"

namespace zel {

namespace reflect {

Object::Object() {}

Object::~Object() {}

void Object::className(const std::string& class_name) { class_name_ = class_name; }

const std::string& Object::className() const { return class_name_; }

int Object::getFieldCount() {
    ClassFactory* factory = utility::Singleton<ClassFactory>::instance();

    return factory->getFieldCount(class_name_);
}

std::shared_ptr<ClassField> Object::getField(int pos) {
    ClassFactory* factory = utility::Singleton<ClassFactory>::instance();

    return factory->getField(class_name_, pos);
}

std::shared_ptr<ClassField> Object::getField(const std::string& field_name) {
    ClassFactory* factory = utility::Singleton<ClassFactory>::instance();

    return factory->getField(class_name_, field_name);
}

void Object::call(const std::string& method_name) {
    ClassFactory* factory = utility::Singleton<ClassFactory>::instance();
    std::shared_ptr<ClassMethod> method = factory->getMethod(class_name_, method_name);

    auto func = method->method();
    typedef std::function<void(decltype(this))> class_method;

    (*((class_method*)func))(this);
}

int Object::call(const std::string& method_name, int a, int b) {
    ClassFactory* factory = utility::Singleton<ClassFactory>::instance();
    std::shared_ptr<ClassMethod> method = factory->getMethod(class_name_, method_name);

    auto func = method->method();
    typedef std::function<int(decltype(this), int, int)> class_method;

    (*((class_method*)func))(this, a, b);
}



ClassFactory::ClassFactory() {}

ClassFactory::~ClassFactory() {}

void ClassFactory::registerClass(const std::string& class_name, create_object mothod) {
    m_class_[class_name] = mothod;
}

Object* ClassFactory::createClass(const std::string& class_name) {

    auto it = m_class_.find(class_name);

    if (it == m_class_.end())
        return nullptr;

    return it->second();
}

void ClassFactory::registerClassField(const std::string& class_name,
                                      const std::string& field_name,
                                      const std::string& field_type,
                                      size_t offset) {

    m_class_fields_[class_name].push_back(
        std::make_shared<ClassField>(field_name, field_type, offset));
}

int ClassFactory::getFieldCount(const std::string& class_name) {
    return m_class_fields_[class_name].size();
}

void ClassFactory::registerClassMethod(const std::string& class_name,
                                       const std::string& method_name,
                                       uintptr_t method) {
    m_class_methods_[class_name].push_back(std::make_shared<ClassMethod>(method_name, method));
}

int ClassFactory::getMethodCount(const std::string& class_name) {
    return m_class_methods_[class_name].size();
}

std::shared_ptr<ClassMethod> ClassFactory::getMethod(const std::string& class_name, int pos) {

    int size = m_class_methods_[class_name].size();
    if (pos < 0 || pos >= size)
        return nullptr;

    return m_class_methods_[class_name][pos];
}

std::shared_ptr<ClassMethod> ClassFactory::getMethod(const std::string& class_name,
                                                     const std::string& method_name) {
    auto methods = m_class_methods_[class_name];
    for (auto it = methods.begin(); it != methods.end(); it++) {
        if ((*it)->name() == method_name) {
            return (*it);
        }
    }

    return nullptr;
}

std::shared_ptr<ClassField> ClassFactory::getField(const std::string& class_name, int pos) {

    int size = m_class_fields_[class_name].size();
    if (pos < 0 || pos >= size)
        return nullptr;

    return m_class_fields_[class_name][pos];
}

std::shared_ptr<ClassField> ClassFactory::getField(const std::string& class_name,
                                                   const std::string& field_name) {

    auto fields = m_class_fields_[class_name];
    for (auto it = fields.begin(); it != fields.end(); it++) {
        if ((*it)->name() == field_name) {
            return (*it);
        }
    }
    return nullptr;
}

} // namespace reflect

} // namespace zel