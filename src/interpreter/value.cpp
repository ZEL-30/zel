#include "value.h"

#include <memory>
#include <sstream>

namespace script {

namespace interpreter {

Value::Value() {}

Value::Value(const std::string& value) : type_(STRING) { value_.string_ = new std::string(value); }

Value::~Value() {}

std::string Value::str() const {

    std::stringstream ss;

    switch (type_) {

    case STRING:
        ss << *value_.string_;
        break;

    default:
        break;
    }

    return ss.str();
}

Value::UValue Value::value() { return value_; }

std::shared_ptr<Value> Value::append(std::shared_ptr<Value> other) {
    if (this->type_ != STRING || other->type_ != STRING)
        return nullptr;

    *this->value_.string_ += *other->value_.string_;
    auto value = std::make_shared<Value>(*value_.string_);
    return value;
}

} // namespace interpreter

} // namespace script