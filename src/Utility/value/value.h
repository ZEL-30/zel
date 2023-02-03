#pragma once

#include <map>
#include <string>

namespace zel {

namespace utility {

class Value {

  public:
    /// @brief 构造函数
    Value();
    Value(bool value);
    Value(int value);
    Value(double value);
    Value(const char* value);
    Value(const std::string& value);

    /// @brief 重载赋值运算符
    Value& operator=(bool value);
    Value& operator=(int value);
    Value& operator=(double value);
    Value& operator=(const char* value);
    Value& operator=(const std::string& value);

    /// @brief 类型转换
    operator bool();
    operator int();
    operator double();
    operator std::string();

  private:
    std::string value_;
};

} // namespace utility
} // namespace zel