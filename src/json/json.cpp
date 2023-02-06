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

Json::Json(const char* value) : type_(JSON_STRING) {
    value_.string_ = new std::string(value);
}

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

Json::Json(const Json& other) { Copy(other); }

Json::~Json() {}

Json::Type Json::type() const { return type_; }

Json const& Json::Null() {
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
        for (auto it = value_.array_->begin(); it != value_.array_->end();
             it++) {
            if (it != value_.array_->begin())
                ss << ", ";
            ss << it->str();
        }

        ss << "]";
        break;
    }

    case Json::Type::JSON_OBJECT: {
        ss << "{";
        for (auto it = value_.object_->begin(); it != value_.object_->end();
             it++) {
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

const Json& Json::Get(int index) const {
    if (type() != JSON_ARRAY)
        throw std::logic_error("function Json::Get [int] requires array value");

    int size = value_.array_->size();
    if (index >= 0 && index < size)
        return value_.array_->at(index);

    return Null();
}

const Json& Json::Get(const char* key) const {
    const std::string name = key;
    return Get(name);
}

const Json& Json::Get(const std::string& key) const {
    if (type() != JSON_OBJECT) {
        throw std::logic_error(
            "function Json::Get [const string &] requires object value");
    }
    std::map<std::string, Json>::const_iterator it = value_.object_->find(key);
    if (it != value_.object_->end()) {
        return it->second;
    }

    return Null();
}

void Json::Set(const Json& other) {}

void Json::Set(bool value) {}

void Json::Set(int value) {}

void Json::Set(double value) {}

void Json::Set(const char* value) {}

void Json::Set(const std::string& value) {}

void Json::Append(const Json& other) {
    if (type_ != JSON_ARRAY) {
        type_ = JSON_ARRAY;
        value_.array_ = new std::vector<Json>();
    }

    (value_.array_)->push_back(other);
}

bool Json::Has(int index) {
    if (type_ != JSON_ARRAY)
        return false;

    int size = value_.array_->size();
    return (index >= 0 && index < size);
}

bool Json::Has(const char* key) {
    std::string name(key);
    return Has(name);
}

bool Json::Has(std::string& key) {
    if (type_ != JSON_OBJECT)
        return false;

    return value_.object_->find(key) != value_.object_->end();
}
void Json::Remove(int index) {
    if (type_ != JSON_ARRAY)
        return;

    int size = value_.array_->size();
    if (index < 0 || index >= size)
        return;

    value_.array_->at(index).Clear();
    value_.array_->erase(value_.array_->begin() + index);
}

void Json::Remove(const char* key) {
    std::string name(key);
    Remove(name);
}

void Json::Remove(std::string& key) {
    if (type_ != JSON_OBJECT)
        return;

    auto it = value_.object_->find(key);
    if (it == value_.object_->end())
        return;

    (*value_.object_)[key].Clear();
    value_.object_->erase(key);
}

bool Json::Parse(const std::string& str) {
    Parser parser;
    if (!parser.LoadString(str))
        return false;

    *this = parser.Parse();

    return true;
}

void Json::Copy(const Json& other) {

    Clear();

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

void Json::Clear() {

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
            delete value_.string_;
            value_.string_ = nullptr;
        }
        break;
    }

    case JSON_ARRAY: {
        if (value_.array_ != nullptr) {
            for (auto it = value_.array_->begin(); it != value_.array_->end();
                 it++) {
                it->Clear();
            }
            delete value_.array_;
            value_.array_ = nullptr;
        }
        break;
    }

    case JSON_OBJECT: {
        if (value_.object_ != nullptr) {
            for (auto it = value_.object_->begin(); it != value_.object_->end();
                 it++) {
                it->second.Clear();
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

bool Json::AsBool() const {

    if (type_ != JSON_BOOL)
        throw std::logic_error("type error, not bool value");

    return value_.bool_;
}

int Json::AsInt() const {

    if (type_ != JSON_INT)
        throw std::logic_error("type error, not int value");

    return value_.int_;
}

double Json::AsDouble() const {
    if (type_ != JSON_DOUBLE)
        throw std::logic_error("type error, not double value");

    return value_.double_;
}

std::string Json::AsString() const {

    if (type_ != JSON_STRING)
        throw std::logic_error("type error, not string value");

    return *value_.string_;
}

bool Json::IsNULL() const { return type_ == JSON_NULL; }

bool Json::IsBool() const { return type_ == JSON_BOOL; }

bool Json::IsInt() const { return type_ == JSON_INT; }

bool Json::IsDouble() const { return type_ == JSON_DOUBLE; }

bool Json::IsString() const { return type_ == JSON_STRING; }

bool Json::IsArray() const { return type_ == JSON_ARRAY; }

bool Json::IsObject() const { return type_ == JSON_OBJECT; }

Json::operator bool() { return AsBool(); }

Json::operator int() { return AsInt(); }

Json::operator double() { return AsDouble(); }

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

const Json& Json::operator[](int index) const { return Get(index); }

Json& Json::operator[](const char* key) {

    std::string name(key);

    return (*this)[name];
}

const Json& Json::operator[](const char* key) const { return Get(key); }

Json& Json::operator[](const std::string& key) {

    if (type_ != JSON_OBJECT) {
        type_ = JSON_OBJECT;
        value_.object_ = new std::map<std::string, Json>();
    }

    return (*value_.object_)[key];
}

const Json& Json::operator[](const std::string& key) const { return Get(key); }

Json& Json::operator=(const Json& other) {
    Copy(other);
    return *this;
}

Json& Json::operator=(bool value) {
    Json other(value);
    Copy(other);
    return *this;
}

Json& Json::operator=(int value) {
    Json other(value);
    Copy(other);
    return *this;
}

Json& Json::operator=(double value) {
    Json other(value);
    Copy(other);
    return *this;
}

Json& Json::operator=(const char* value) {
    Json other(value);
    Copy(other);
    return *this;
}

Json& Json::operator=(const std::string& value) {
    Json other(value);
    Copy(other);
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
std::vector<Json>::const_iterator Json::begin() const {
    return value_.array_->begin();
}

std::vector<Json>::iterator Json::end() { return value_.array_->end(); }
std::vector<Json>::const_iterator Json::end() const {
    return value_.array_->end();
}

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