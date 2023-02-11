#include "lexer.h"

#include <cassert>
#include <vector>

namespace zel {

namespace interpreter {

Lexer::Lexer(const std::string& source) {
    source_ = source;
    index_ = 0;
}

Lexer::~Lexer() {}

std::vector<Token> Lexer::Tokenize() {

    std::vector<Token> v_tokens;

    if (source_ == "")
        return {};

    while (source_[index_] != '\0') {

        switch (source_[index_]) {

        case '\n':{
            v_tokens.push_back(Token("EOL", Token::END_OF_LINE));
            advance();
            break;
        }


        // 识别注释
        case ';':
        case '/': {
            v_tokens.push_back(comment());
            break;
        }
        case ' ':
        case '.':
        case '\t':
        case '\r': {
            advance();
            break;
        }

        // 识别等号
        case '=': {
            v_tokens.push_back(Token("=", Token::EQUAL));
            advance();
            break;
        }

        // 识别字符串、标识符或关键字
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '*':
        case 'a':
        case 'A':
        case 'b':
        case 'B':
        case 'c':
        case 'C':
        case 'd':
        case 'D':
        case 'e':
        case 'E':
        case 'f':
        case 'F':
        case 'g':
        case 'G':
        case 'h':
        case 'H':
        case 'i':
        case 'I':
        case 'j':
        case 'J':
        case 'k':
        case 'K':
        case 'l':
        case 'L':
        case 'm':
        case 'M':
        case 'n':
        case 'N':
        case 'o':
        case 'O':
        case 'p':
        case 'P':
        case 'q':
        case 'Q':
        case 'r':
        case 'R':
        case 's':
        case 'S':
        case 't':
        case 'T':
        case 'u':
        case 'U':
        case 'v':
        case 'V':
        case 'w':
        case 'W':
        case 'x':
        case 'X':
        case 'y':
        case 'Y':
        case 'z':
        case 'Z': {
            v_tokens.push_back(identifierOrKeywords());
            break;
        }

        // 识别圆括号
        case '(': {
            v_tokens.push_back(Token("(", Token::LPAREN));
            advance();
            break;
        }
        case ')': {
            v_tokens.push_back(Token(")", Token::RPAREN));
            advance();
            break;
        }

        // 识别中括号
        case '[': {
            v_tokens.push_back(Token(")", Token::LBRACKET));
            advance();
            break;
        }
        case ']': {
            v_tokens.push_back(Token(")", Token::RBRACKET));
            advance();
            break;
        }

        // 识别逗号
        case ',': {
            v_tokens.push_back(Token(",", Token::COMMA));
            advance();
            break;
        }

        // 没有匹配到， 非法字符错误
        default: {
            printf("unexpected char '%c'\n", currentChar());
            advance();
            break;
        }
        }
    }

    v_tokens.push_back(Token("EOS", Token::END_OF_SOURCE));

    return v_tokens;
}

Token Lexer::identifierOrKeywords() {
    std::string temp;

    while (isLetter(currentChar()) || isNumber(currentChar()) || currentChar() == '_' ||
           currentChar() == '.' || currentChar() == '*' || currentChar() == '-') {
        temp += currentChar();
        advance();
    }

    if (temp.length() <= 10) {
        return Token(temp, Token::IDENTIFIER);
    }

    return Token(temp, Token::STRING);
}

Token Lexer::comment() {

    std::string temp;

    while (currentChar() != '\n' && currentChar() != '\0') {
        temp += currentChar();
        advance();
    }

    return Token(temp, Token::COMMENT);
}

char Lexer::currentChar() { return source_[index_]; }

char Lexer::advance() { return source_[index_++]; }

bool Lexer::isLetter(char ch) { return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'); }

bool Lexer::isNumber(char ch) { return (ch >= '0' && ch <= '9'); }

} // namespace interpreter

} // namespace zel
