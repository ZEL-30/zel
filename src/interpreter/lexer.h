#pragma once

#include "token.h"

#include <memory>
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
    std::vector<std::shared_ptr<Token>> Tokenize();

  private:
    char advance();
    char currentChar();

    bool isLetter(char ch);
    bool isNumber(char ch);

    void skipWhiteSpace();

    std::shared_ptr<Token> identifierOrKeywords();
    std::shared_ptr<Token> comment();

  private:
    std::string source_;
    int index_;
};

} // namespace interpreter

} // namespace zel
