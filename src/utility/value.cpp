/// @file value.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-02-02
/// @copyright Copyright (c) 2023 ZEL

#include "value.h"

namespace zel {

namespace utility {

Value::Value() : type_(V_NULL) {}

Value::~Value() {}

Value::Value(bool value) : type_(V_BOOL) { *this = value; }

Value::Value(int value) : type_(V_INT) { *this = value; }

Value::Value(double value) : type_(V_DOUBLE) { *this = value; }

Value::Value(const char* value) : type_(V_STRING), value_(value) {}

Value::Value(const std::string& value) : type_(V_STRING), value_(value) {}

Value::Type Value::type() const { return type_; }

void Value::type(Type type) { type_ = type; }

std::string Value::str() const { return value_; }

// const std::basic_string<char>::value_type* Value::c_str() const { return value_.c_str(); }

bool Value::isNull() const { return type_ == V_NULL; }

bool Value::isInt() const { return type_ == V_INT; }

bool Value::isDouble() const { return type_ == V_DOUBLE; }

bool Value::isString() const { return type_ == V_STRING; }

bool Value::asBool() const {
    if (value_ == "true")
        return true;
    else if (value_ == "false")
        return false;

    return false;
}

int Value::asInt() const { return std::stoi(value_); }

double Value::asDouble() const { return std::stof(value_); }

std::string Value::asString() const { return value_; }

Value& Value::operator=(bool value) {
    type_ = V_BOOL;
    value_ = value ? "true" : "false";

    return *this;
}

Value& Value::operator=(int value) {
    type_ = V_INT;
    value_ = std::to_string(value);

    return *this;
}

Value& Value::operator=(double value) {
    type_ = V_DOUBLE;
    value_ = std::to_string(value);

    return *this;
}

Value& Value::operator=(const char* value) {
    type_ = V_STRING;
    value_ = value;

    return *this;
}

Value& Value::operator=(const std::string& value) {
    type_ = V_STRING;
    value_ = value;

    return *this;
}

bool Value::operator==(const Value& other) {
    if (type_ != other.type_)
        return false;

    return value_ == other.value_;
}

bool Value::operator!=(const Value& other) { return !(value_ == other.value_); }

Value::operator bool() { return asBool(); }

Value::operator int() { return asInt(); }

Value::operator double() { return asDouble(); }

Value::operator std::string() { return asString(); }

Value::operator std::string() const { return asString(); }

} // namespace utility
} // namespace zel