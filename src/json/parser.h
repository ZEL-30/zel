#pragma once

#include "json.h"

#include <iostream>

namespace zel {
namespace json {

class Parser {

  public:
    Parser();
    ~Parser();

    bool loadString(const std::string& str);
    bool loadFile(const std::string& filename);

    Json parse();

  private:
    void skipWhiteSpace();

    char getNextChar();

    Json parseNull();
    Json parseBool();
    Json parseNumber();
    std::string parseString();
    Json parseArray();
    Json parseObject();

  private:
    std::string str_; // json buffer
    int index_;       // 下标
};

} // namespace json
} // namespace zel