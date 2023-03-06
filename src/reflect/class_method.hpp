#pragma once

#include <string>

namespace zel {

namespace reflect {

class ClassMethod {

  public:
    ClassMethod() : name_(""), method_(0) {}
    ClassMethod(const std::string& name, uintptr_t method) : name_(name), method_(method) {}
    ~ClassMethod() {}

    const std::string& name() { return name_; }

    uintptr_t method() { return method_; }

  private:
    std::string name_;
    uintptr_t method_;
};

} // namespace reflect

} // namespace zel
