#pragma once
#include <memory>
#include <string>

namespace zel {

namespace interpreter {

class Value {

  public:
    enum Type {
        STRING,
    };

    Value();
    Value(const std::string & value);
    ~Value();

    std::string str() const;

    std::shared_ptr<Value> append(std::shared_ptr<Value> other);

    void clear();

  private:

  private:
  
    struct Apdu {
        std::string commend;
        std::string responce;
        std::string expected;
    };

    union UValue {
        std::string *string_;
        Apdu *apdu_;
    };

    Type type_;
    UValue value_;
};

} // namespace interpreter

} // namespace zel