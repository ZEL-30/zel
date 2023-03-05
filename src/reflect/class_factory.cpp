#include "class_factory.h"

namespace zel {

namespace reflect {

ClassFactory::ClassFactory() {}

ClassFactory::~ClassFactory() {}

void ClassFactory::register_class(const std::string &class_name, create_object mothod) {
    m_class_[class_name] = mothod;
}

void* ClassFactory::create_class(const std::string &class_name) {

    auto it = m_class_.find(class_name);

    if (it == m_class_.end()) 
        return nullptr;

    return it->second();
}



} // namespace reflect

} // namespace zel