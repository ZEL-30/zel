#include "token.h"

namespace zel {

namespace interpreter {

Token::Token() {}

Token::Token(const std::string& value, Type type) {
    value_ = value;
    type_ = type;
}

Token::~Token() {}

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
        str = "LPAREN";
        break;

    case RPAREN:
        str = "RPAREN";
        break;

    case PLUS:
        str = "PLUS";
        break;

    case LBRACKET:
        str = "LBRACKET";
        break;

    case RBRACKET:
        str = "RBRACKET";
        break;

    case EQUAL:
        str = "EQUAL";
        break;

    case COMMA:
        str = "COMMA";
        break;

    case COMMENT:
        str = "COMMENT: " + value_;
        break;

    case ERRORS:
        str = "ERRORS";
        break;

    case END_OF_SOURCE:
        str = "EOS";
        break;

    case END_OF_LINE:
        str = "EOL";
        break;

    default:
        break;
    }

    return str;
}

Token::Type Token::type() { return type_; }

bool Token::matches(Type type, std::string keywords) { return true; }

} // namespace interpreter

} // namespace zel