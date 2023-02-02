/// @file value.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-02-02
/// @copyright Copyright (c) 2023 ZEL

#include "value.h"

namespace zel {

namespace utility {

CValue::CValue() {}

CValue::CValue(bool value) { *this = value; }

CValue::CValue(int value) { *this = value; }

CValue::CValue(double value) { *this = value; }

CValue::CValue(const char* value) { *this = value; }

CValue::CValue(const std::string& value) { *this = value; }

CValue& CValue::operator=(bool value) {
    value_ = value ? "true" : "false";
    return *this;
}

CValue& CValue::operator=(int value) {
    value_ = std::to_string(value);
    return *this;
}

CValue& CValue::operator=(double value) {
    value_ = std::to_string(value);
    return *this;
}

CValue& CValue::operator=(const char* value) {
    value_ = value;
    return *this;
}

CValue& CValue::operator=(const std::string& value) {
    value_ = value;
    return *this;
}

CValue::operator bool() { return value_ == "true"; }

CValue::operator int() { return std::stoi(value_); }

CValue::operator double() { return std::stof(value_); }

CValue::operator std::string() { return value_; }

} // namespace utility
} // namespace zel