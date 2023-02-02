#pragma once

#include <map>
#include <string>

namespace zel {

namespace utility {

class CValue {

  public:
    /// @brief 构造函数
    CValue();
    CValue(bool value);
    CValue(int value);
    CValue(double value);
    CValue(const char* value);
    CValue(const std::string& value);

    /// @brief 重载赋值运算符
    CValue& operator=(bool value);
    CValue& operator=(int value);
    CValue& operator=(double value);
    CValue& operator=(const char* value);
    CValue& operator=(const std::string& value);

    /// @brief 类型转换
    operator bool();
    operator int();
    operator double();
    operator std::string();

  private:
    std::string value_;
};

typedef std::map<std::string, CValue> Section;

} // namespace utility
} // namespace zel