#pragma once

#include <string>

namespace zel {

namespace interpreter {

class Token {

  public:
    enum Type {
        STRING, // 字符串

        IDENTIFIER, // 标识符
        KEYWORDS,   // 关键字

        LPAREN, // (
        RPAREN, // )

        PLUS, // 默认字符串操作符

        LBRACKET, // [
        RBRACKET, // ]

        EQUAL, // =

        COMMA, // ,

        COMMENT, // 注释

        ERRORS, // 错误

        END_OF_SOURCE, // EOS
        END_OF_LINE,   // EOL
    };

    Token();
    Token(const std::string& value, Type type);
    ~Token();

    Type type();

    /// @brief 节点序列化 C++ string
    std::string str() const;

    bool matches(Type type, std::string value);

  private:
    Type type_;
    std::string value_;
};

} // namespace interpreter

} // namespace zel
