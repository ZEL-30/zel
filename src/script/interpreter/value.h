#pragma once

#include "../function/string.h"

#include <memory>
#include <string>

namespace script {

namespace interpreter {

class Value {

  public:
    enum Type { STRING, APDU };

    struct Apdu {
        std::string commend;
        std::string responce;
        std::string expected;
    };

    struct Function {
      std::string func_name;
      // std::shared_ptr<Value>(function::*func_body)(std::vector<std::shared_ptr<Value>>);
    };

    union UValue {
        std::string* string_;
        Apdu* apdu_;
    };

    Value();
    Value(const std::string& value);
    ~Value();

    std::string str() const;

    Value::UValue value();

    std::shared_ptr<Value> append(std::shared_ptr<Value> other);

    void clear();

  private:
  private:
    Type type_;
    UValue value_;
};

} // namespace interpreter

} // namespace script