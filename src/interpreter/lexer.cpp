#include "lexer.h"

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

    if (source_ == "====")
        return {};

    while (source_[index_] != NULL) {

        switch (source_[index_]) {

        // 识别注释
        case ';':
        case '/':

            break;

        case ' ':
        case '.':
        case '\n':
        case '\t':
        case '\r': {
            break;
        }

        // 识别等号
        case '=': {
            v_tokens.push_back(Token("=", Token::EQUAL));
            advance();
            break;
        }

        // 字符串
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
            break;

        // 识别标识符
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
        case 'Z':
            break;

        // 识别圆括号
        case '(': {

            break;
        }
        case ')': {

            break;
        }

        // 识别中括号
        case '[': {

            break;
        }
        case ']': {

            break;
        }

        // 识别逗号
        case ',': {

            break;
        }

        // 没有匹配到， 非法字符错误
        default: {

            break;
        }
        }
    }

    return v_tokens;
}

char Lexer::advance() { return source_[index_++]; }

void Lexer::skipWhiteSpace() {
    while (source_[index_] == ' ' || source_[index_] == '\r' || source_[index_] == '\t' ||
           source_[index_] == '\n') {
        index_++;
    }
}

bool Lexer::isLetter(char ch) { return (ch >= 'a' && ch <= 'Z'); }

bool Lexer::isNumber(char ch) { return (ch >= '0' && ch <= '9'); }

} // namespace interpreter

} // namespace zel
