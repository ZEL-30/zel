#pragma once

#include "token.h"

#include <string>
#include <vector>

namespace zel {

namespace interpreter {

class Lexer {

  public:
    Lexer(const std::string& source);
    ~Lexer();

    /// @brief 进行词法分析，获取所有 tokens.
    /// @return 所有 tokens.
    std::vector<Token> Tokenize();

  private:
    char advance();

    void skipWhiteSpace();

    bool isLetter(char ch);
    bool isNumber(char ch);

    Token string();

  private:
    std::string source_;
    int index_;
};

} // namespace interpreter

} // namespace zel
