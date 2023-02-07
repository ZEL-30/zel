/// @file json.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-02-07
/// @copyright Copyright (c) 2023 ZEL

#include "json.h"

#include "parser.h"

#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace zel {

namespace json {

Json::Json() : type_(JSON_NULL) {}

Json::Json(bool value) : type_(JSON_BOOL) { value_.bool_ = value; }

Json::Json(int value) : type_(JSON_INT) { value_.int_ = value; }

Json::Json(double value) : type_(JSON_DOUBLE) { value_.double_ = value; }

Json::Json(const char* value) : type_(JSON_STRING) { value_.string_ = new std::string(value); }

Json::Json(const std::string& value) : type_(JSON_STRING) {
    value_.string_ = new std::string(value);
}

Json::Json(Type type) : type_(type) {

    switch (type) {

    case JSON_NULL:
        break;

    case JSON_BOOL:
        value_.bool_ = false;
        break;

    case JSON_INT:
        value_.int_ = 0;
        break;

    case JSON_DOUBLE:
        value_.double_ = 0.0;
        break;

    case JSON_STRING:
        value_.string_ = new std::string("");
        break;

    case JSON_ARRAY:
        value_.array_ = new std::vector<Json>();
        break;

    case JSON_OBJECT:
        value_.object_ = new std::map<std::string, Json>();
        break;

    default:
        break;
    }
}

Json::Json(const Json& other) { copy(other); }

Json::~Json() {}

Json::Type Json::type() const { return type_; }

Json const& Json::null() {
    static const Json null;
    return null;
}

std::string Json::str() const {

    std::stringstream ss;

    switch (type_) {

    case Json::Type::JSON_NULL:
        ss << "null";
        break;

    case Json::Type::JSON_BOOL: {
        std::string str = value_.bool_ ? "true" : "false";
        ss << str;
        break;
    }

    case Json::Type::JSON_INT:
        ss << value_.int_;
        break;

    case Json::Type::JSON_DOUBLE:
        ss << value_.double_;
        break;

    case Json::Type::JSON_STRING:
        ss << "\"" << *value_.string_ << "\"";
        break;

    case Json::Type::JSON_ARRAY: {
        ss << "[";
        for (auto it = value_.array_->begin(); it != value_.array_->end(); it++) {
            if (it != value_.array_->begin())
                ss << ", ";
            ss << it->str();
        }

        ss << "]";
        break;
    }

    case Json::Type::JSON_OBJECT: {
        ss << "{";
        for (auto it = value_.object_->begin(); it != value_.object_->end(); it++) {
            if (it != value_.object_->begin())
                ss << ", ";
            ss << "\"" << it->first << "\" : " << it->second.str();
        }

        ss << "}";
        break;
    }

    default:
        break;
    }

    return ss.str();
}

const Json& Json::get(int index) const {
    if (type() != JSON_ARRAY)
        throw std::logic_error("function Json::get [int] requires array value");

    int size = value_.array_->size();
    if (index >= 0 && index < size)
        return value_.array_->at(index);

    return null();
}

const Json& Json::get(const char* key) const {
    const std::string name = key;
    return get(name);
}

const Json& Json::get(const std::string& key) const {
    if (type() != JSON_OBJECT) {
        throw std::logic_error("function Json::get [const string &] requires object value");
    }
    std::map<std::string, Json>::const_iterator it = value_.object_->find(key);
    if (it != value_.object_->end()) {
        return it->second;
    }

    return null();
}

void Json::set(const Json& other) {}

void Json::set(bool value) {}

void Json::set(int value) {}

void Json::set(double value) {}

void Json::set(const char* value) {}

void Json::set(const std::string& value) {}

void Json::append(const Json& other) {
    if (type_ != JSON_ARRAY) {
        type_ = JSON_ARRAY;
        value_.array_ = new std::vector<Json>();
    }

    (value_.array_)->push_back(other);
}

bool Json::has(int index) {
    if (type_ != JSON_ARRAY)
        return false;

    int size = value_.array_->size();
    return (index >= 0 && index < size);
}

bool Json::has(const char* key) {
    std::string name(key);
    return has(name);
}

bool Json::has(std::string& key) {
    if (type_ != JSON_OBJECT)
        return false;

    return value_.object_->find(key) != value_.object_->end();
}
void Json::remove(int index) {
    if (type_ != JSON_ARRAY)
        return;

    int size = value_.array_->size();
    if (index < 0 || index >= size)
        return;

    value_.array_->at(index).clear();

    std::cout << "1 :" <<value_.array_->at(3).str() << std::endl;
    value_.array_->erase(value_.array_->begin() + index);

    std::cout << "2 :" <<value_.array_->at(2).str() << std::endl;
}

void Json::remove(const char* key) {
    std::string name(key);
    remove(name);
}

void Json::remove(std::string& key) {
    if (type_ != JSON_OBJECT)
        return;

    auto it = value_.object_->find(key);
    if (it == value_.object_->end())
        return;

    (*value_.object_)[key].clear();
    value_.object_->erase(key);
}

bool Json::parse(const std::string& str) {
    Parser parser;
    if (!parser.loadString(str))
        return false;

    *this = parser.parse();

    return true;
}

void Json::copy(const Json& other) {

    clear();

    type_ = other.type_;

    switch (type_) {

    case JSON_NULL:
        break;

    case JSON_BOOL:
        value_.bool_ = other.value_.bool_;
        break;

    case JSON_INT:
        value_.int_ = other.value_.int_;
        break;

    case JSON_DOUBLE:
        value_.double_ = other.value_.double_;
        break;

    case JSON_STRING:
        value_.string_ = other.value_.string_;
        break;

    case JSON_ARRAY:
        value_.array_ = other.value_.array_;
        break;

    case JSON_OBJECT:
        value_.object_ = other.value_.object_;
        break;

    default:
        break;
    }
}

void Json::clear() {

    switch (type_) {

    case JSON_NULL:
        break;

    case JSON_BOOL:
        value_.bool_ = false;
        break;

    case JSON_INT:
        value_.int_ = 0;
        break;

    case JSON_DOUBLE:
        value_.double_ = 0.0;
        break;

    case JSON_STRING: {
        if (value_.string_ != nullptr) {
            printf("%s\n", value_.string_->c_str());
            delete value_.string_;
            value_.string_ = nullptr;
        }
        break;
    }

    case JSON_ARRAY: {
        if (value_.array_ != nullptr) {
            for (auto it = value_.array_->begin(); it != value_.array_->end(); it++) {
                it->clear();
            }
            delete value_.array_;
            value_.array_ = nullptr;
        }
        break;
    }

    case JSON_OBJECT: {
        if (value_.object_ != nullptr) {
            for (auto it = value_.object_->begin(); it != value_.object_->end(); it++) {
                it->second.clear();
            }
            delete value_.object_;
            value_.object_ = nullptr;
        }
        break;
    }

    default:
        break;
    }

    type_ = JSON_NULL;
}

bool Json::asBool() const {

    if (type_ != JSON_BOOL)
        throw std::logic_error("type error, not bool value");

    return value_.bool_;
}

int Json::asInt() const {

    if (type_ != JSON_INT)
        throw std::logic_error("type error, not int value");

    return value_.int_;
}

double Json::asDouble() const {
    if (type_ != JSON_DOUBLE)
        throw std::logic_error("type error, not double value");

    return value_.double_;
}

std::string Json::asString() const {

    if (type_ != JSON_STRING)
        throw std::logic_error("type error, not string value");

    return *value_.string_;
}

bool Json::isNULL() const { return type_ == JSON_NULL; }

bool Json::isBool() const { return type_ == JSON_BOOL; }

bool Json::isInt() const { return type_ == JSON_INT; }

bool Json::isDouble() const { return type_ == JSON_DOUBLE; }

bool Json::isString() const { return type_ == JSON_STRING; }

bool Json::isArray() const { return type_ == JSON_ARRAY; }

bool Json::isObject() const { return type_ == JSON_OBJECT; }

Json::operator bool() { return asBool(); }

Json::operator int() { return asInt(); }

Json::operator double() { return asDouble(); }

Json::operator std::string() {

    if (type_ != JSON_STRING)
        throw std::logic_error("type error, not string value");

    return *value_.string_;
}

Json::operator std::string() const {

    if (type_ != JSON_STRING)
        throw std::logic_error("type error, not string value");

    return *value_.string_;
}

Json::operator std::basic_string<char>::value_type*() const {
    return (std::basic_string<char>::value_type*)value_.string_->c_str();
}

Json& Json::operator[](int index) {
    if (type_ != JSON_ARRAY) {
        type_ = JSON_ARRAY;
        value_.array_ = new std::vector<Json>();
    }

    if (index < 0) {
        throw std::logic_error("index less than zero");
    }

    int size = value_.array_->size();
    if (index >= size) {
        for (int i = size; i <= index; i++) {
            value_.array_->push_back(Json());
        }
    }

    return value_.array_->at(index);
}

const Json& Json::operator[](int index) const { return get(index); }

Json& Json::operator[](const char* key) {

    std::string name(key);

    return (*this)[name];
}

const Json& Json::operator[](const char* key) const { return get(key); }

Json& Json::operator[](const std::string& key) {

    if (type_ != JSON_OBJECT) {
        type_ = JSON_OBJECT;
        value_.object_ = new std::map<std::string, Json>();
    }

    return (*value_.object_)[key];
}

const Json& Json::operator[](const std::string& key) const { return get(key); }

Json& Json::operator=(const Json& other) {
    copy(other);
    return *this;
}

Json& Json::operator=(bool value) {
    Json other(value);
    copy(other);
    return *this;
}

Json& Json::operator=(int value) {
    Json other(value);
    copy(other);
    return *this;
}

Json& Json::operator=(double value) {
    Json other(value);
    copy(other);
    return *this;
}

Json& Json::operator=(const char* value) {
    Json other(value);
    copy(other);
    return *this;
}

Json& Json::operator=(const std::string& value) {
    Json other(value);
    copy(other);
    return *this;
}

bool Json::operator==(const Json& other) {

    if (type_ != other.type_)
        return false;

    switch (type_) {

    case Json::Type::JSON_NULL:
        return true;

    case Json::Type::JSON_BOOL:
        return value_.bool_ == other.value_.bool_;

    case Json::Type::JSON_INT:

        return value_.int_ == other.value_.int_;

    case Json::Type::JSON_DOUBLE:
        return value_.double_ == other.value_.double_;

    case Json::Type::JSON_STRING:
        return (*value_.string_) == (*other.value_.string_);
        break;

    case Json::Type::JSON_ARRAY:
        return value_.array_ == other.value_.array_;

    case Json::Type::JSON_OBJECT:
        return value_.object_ == other.value_.object_;

    default:
        break;
    }

    return false;
}

bool Json::operator==(bool value) {
    Json other = value;
    return (*this == other);
}

bool Json::operator==(int value) {
    Json other = value;
    return (*this == other);
}

bool Json::operator==(double value) {
    Json other = value;
    return (*this == other);
}

bool Json::operator==(const char* value) {
    Json other = value;
    return (*this == other);
}

bool Json::operator==(const std::string& value) {
    Json other = value;
    return (*this == other);
}

bool Json::operator!=(const Json& other) { return !(*this == other); }

bool Json::operator!=(bool value) {
    Json other = value;
    return (*this != other);
}

bool Json::operator!=(int value) {
    Json other = value;
    return (*this != other);
}

bool Json::operator!=(double value) {
    Json other = value;
    return (*this != other);
}

bool Json::operator!=(const char* value) {
    Json other = value;
    return (*this != other);
}

bool Json::operator!=(const std::string& value) {
    Json other = value;
    return (*this != other);
}

std::vector<Json>::iterator Json::begin() { return value_.array_->begin(); }

std::vector<Json>::const_iterator Json::begin() const { return value_.array_->begin(); }

std::vector<Json>::iterator Json::end() { return value_.array_->end(); }

std::vector<Json>::const_iterator Json::end() const { return value_.array_->end(); }

int Json::size() {

    switch (type()) {
    case JSON_ARRAY:
        return value_.array_->size();

    case JSON_OBJECT:
        return value_.object_->size();

    default:
        break;
    }

    throw std::logic_error("function Json::size value type error");
}

bool Json::empty() {

    switch (type()) {

    case JSON_NULL:
        return true;

    case JSON_ARRAY:
        return value_.array_->empty();

    case JSON_OBJECT:
        return value_.object_->empty();

    default:
        break;
    }

    throw std::logic_error("function Json::empty value type error");
}

} // namespace json

} // namespace zel