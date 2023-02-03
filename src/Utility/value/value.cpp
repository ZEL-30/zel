/// @file value.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-02-02
/// @copyright Copyright (c) 2023 ZEL

#include "value.h"

namespace zel {

namespace utility {

Value::Value() {}

Value::Value(bool value) { *this = value; }

Value::Value(int value) { *this = value; }

Value::Value(double value) { *this = value; }

Value::Value(const char* value) { *this = value; }

Value::Value(const std::string& value) { *this = value; }

Value& Value::operator=(bool value) {
    value_ = value ? "true" : "false";
    return *this;
}

Value& Value::operator=(int value) {
    value_ = std::to_string(value);
    return *this;
}

Value& Value::operator=(double value) {
    value_ = std::to_string(value);
    return *this;
}

Value& Value::operator=(const char* value) {
    value_ = value;
    return *this;
}

Value& Value::operator=(const std::string& value) {
    value_ = value;
    return *this;
}

Value::operator bool() { return value_ == "true"; }

Value::operator int() { return std::stoi(value_); }

Value::operator double() { return std::stof(value_); }

Value::operator std::string() { return value_; }

} // namespace utility
} // namespace zel