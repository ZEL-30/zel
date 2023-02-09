#include "token.h"


namespace zel {

namespace interpreter {

Token::Token() {}

Token::Token(const std::string& value, Type type) {
    value_ = value;
    type_ = type;
}

std::string Token::str() const {

    std::string str;

    switch (type_) {

    case STRING:
        break;

    case IDENTIFIER:
    case KEYWORDS:
    case LPAREN:
    case RPAREN:
    case PLUS:
    case LBRACKET:
    case RBRACKET:
    case EQUAL:
        str = "EQUAL";
        break;
    case COMMA:
    case ERRORS:
    case END_OF_SOURCE:
        break;
    }

    return str;
}

Token::~Token() {}

} // namespace interpreter

} // namespace zel