#pragma once

#include "json.h"

#include <iostream>

namespace zel {
namespace json {

class Parser {

  public:
    Parser();
    ~Parser();

    bool LoadString(const std::string& str);
    bool LoadFile(const std::string& filename);

    Json Parse();

  private:
    void SkipWhiteSpace();

    char GetNextChar();

    Json ParseNull();
    Json ParseBool();
    Json ParseNumber();
    std::string ParseString();
    Json ParseArray();
    Json ParseObject();

  private:
    std::string str_; // json buffer
    int index_;       // 下标
};

} // namespace json
} // namespace zel