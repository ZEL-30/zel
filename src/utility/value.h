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

    /// @brief 序列化为 C++ string 或 C char
    std::string str() const;

    Type type() const;
    void type(Type type);

    bool isNull() const;
    bool isInt() const;
    bool isDouble() const;
    bool isString() const;

    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    std::string asString() const;

    /// @brief 重载赋值运算符
    Value& operator=(bool value);
    Value& operator=(int value);
    Value& operator=(double value);
    Value& operator=(const char* value);
    Value& operator=(const std::string& value);

    /// @brief 重载判断操作符
    bool operator==(const Value& other);
    bool operator!=(const Value& other);

    /// @brief 类型转换
    operator bool();
    operator int();
    operator double();
    operator std::string();
    operator std::string() const;

  private:
    Type type_;
    std::string value_;
};

} // namespace utility
} // namespace zel