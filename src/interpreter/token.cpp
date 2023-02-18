#include "token.h"

namespace zel {

namespace interpreter {

Token::Token() {}

Token::Token(const std::string& value, Type type) {
    value_ = value;
    type_ = type;
}

Token::~Token() {}

Token::Type Token::type() { return type_; }

std::string Token::str() const {

    std::string str;

    switch (type_) {

    case STRING:
        str = "STRING: " + value_;
        break;

    case IDENTIFIER:
        str = "IDENTIFIER: " + value_;
        break;

    case KEYWORDS:
        str = "KEYWORDS: " + value_;
        break;

    case LPAREN:
        str = "LPAREN: " + value_;
        break;

    case RPAREN:
        str = "RPAREN: " + value_;
        break;

    case PLUS:
        str = "PLUS: " + value_;
        break;

    case LBRACKET:
        str = "LBRACKET: " + value_;
        break;

    case RBRACKET:
        str = "RBRACKET: " + value_;
        break;

    case EQUAL:
        str = "EQUAL: " + value_;
        break;

    case COMMA:
        str = "COMMA: " + value_;
        break;

    case COMMENT:
        str = "COMMENT: " + value_;
        break;

    case ERRORS:
        str = "ERRORS: " + value_;
        break;

    case END_OF_SOURCE:
        str = "EOS: " + value_;
        break;

    case END_OF_LINE:
        str = "EOL: " + value_;
        break;

    default:
        break;
    }

    return str;
}

bool Token::matches(Type type, std::string value) { return type_ == type && value_ == value; }

} // namespace interpreter

} // namespace zel