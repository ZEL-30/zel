#pragma once

#include <map>
#include <string>

namespace zel {

namespace utility {

class Value {

  public:
    enum Type { V_NULL = 0, V_BOOL, V_INT, V_DOUBLE, V_STRING };

    Value();
    Value(bool value);
    Value(int value);
    Value(double value);
    Value(const char* value);
    Value(const std::string& value);
    ~Value();

    /// @brief 序列化为 c++ string
    std::string str() const;

    /// @brief 序列化为 c char
    const std::basic_string<char>::value_type* c_str() const;

    /// @brief 重载赋值运算符
    Value& operator=(bool value);
    Value& operator=(int value);
    Value& operator=(double value);
    Value& operator=(const char* value);
    Value& operator=(const std::string& value);

    /// @brief 重载判断操作符
    bool operator==(const Value& other);
    bool operator!=(const Value& other);

    bool IsNull() const;
    bool IsInt() const;
    bool IsDouble() const;
    bool IsString() const;

    /// @brief 类型转换
    operator bool();
    operator int();
    operator double();
    operator std::string();
    operator std::string() const;
    // operator std::basic_string<char>::value_type*() const;

    bool AsBool() const;
    int AsInt() const;
    double AsDouble() const;
    std::string AsString() const;

    Type type() const;
    void type(Type type);

  private:
    Type type_;
    std::string value_;
};

} // namespace utility
} // namespace zel