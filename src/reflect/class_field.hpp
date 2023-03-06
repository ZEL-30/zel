#pragma once

#include <string>

namespace zel {

namespace reflect {

class ClassField {

  public:
    ClassField() : name_(""), type_(""), offset_(0) {}
    ClassField(const std::string& name, const std::string& type, size_t offset)
        : name_(name),
          type_(type),
          offset_(offset) {}
    ~ClassField() {}

    const std::string& name() { return name_; }
    const std::string& type() { return type_; }
    size_t offset() { return offset_; }

  private:
    std::string name_;
    std::string type_;
    size_t offset_;
};

} // namespace reflect

} // namespace zel