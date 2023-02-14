#include "lexer.h"

CLexer::CLexer(char* file_name, char* source) {

    InitKeywords();
    strcpy_s(source_, source);
    strcpy_s(file_name_, file_name);
    pos_ = new CPosition(-1, 0, -1, file_name_, source_);
    current_char_ = NULL;

    v_tokens_.clear();
    v_tokens_list_.clear();

    Advance();
}

std::vector<std::vector<CToken*>> CLexer::MakeTokensList() {

    CError* Error = NULL;

    std::vector<CToken*> v_tokens;
    std::vector<std::vector<CToken*>> v_tokens_list;

    // 1. 遍历text
    // 2. 遍历的过程中， 分别判断获取的内容
    while (current_char_ != NULL) {

        switch (current_char_) {

        // 处理换行
        case '\n': {
            MakeEndOfSource();
            break;
        }

        // 识别注释
        case ';':
        case '/':
            MakeComments();
            break;

        case ' ':
        case '.':
        case '\t':
        case '\r': {
            Advance();
            break;
        }

        // 识别等号
        case '=': {
            v_tokens_.push_back(new CToken(CToken::ScriptTokenType::EQUAL, pos_));
            Advance();
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
            v_tokens_.push_back(MakeString());
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
            v_tokens_.push_back(MakeStringOrIdentifier());
            break;

        // 识别圆括号
        case '(': {
            v_tokens_.push_back(new CToken(CToken::ScriptTokenType::LPAREN, pos_));
            Advance();
            break;
        }
        case ')': {
            v_tokens_.push_back(new CToken(CToken::ScriptTokenType::RPAREN, pos_));
            Advance();
            break;
        }

        // 识别中括号
        case '[': {
            v_tokens_.push_back(new CToken(CToken::ScriptTokenType::LBRACKET, pos_));
            Advance();
            break;
        }
        case ']': {
            v_tokens_.push_back(new CToken(CToken::ScriptTokenType::RBRACKET, pos_));
            Advance();
            break;
        }

        // 识别逗号
        case ',': {
            v_tokens_.push_back(new CToken(CToken::ScriptTokenType::COMMA, pos_));
            Advance();
            break;
        }

        // 没有匹配到， 非法字符错误
        default: {
            CPosition* pos_start = pos_->Copy();
            v_tokens_.push_back(new CToken(CToken::ScriptTokenType::ERRORS, pos_));

            char error_char[256];
            memset(error_char, 0, sizeof(error_char));
            sprintf(error_char, "'%c'", current_char_);
            Error = new CIllegalCharError(pos_start, pos_, error_char);
            printf("%s\n", Error->AsString());
            Advance();
            break;
        }
        }
    }

    MakeEndOfSource();

    return v_tokens_list_;
}

void CLexer::PrintTokens() {
    printf("tokens:  [");
    for (int i = 0; i < v_tokens_.size(); i++) {
        printf("%s", v_tokens_[i]->PrintToken());
        if (i != v_tokens_.size() - 1)
            printf(", ");
    }
    printf("]\n");
}

/// @brief 预读
void CLexer::Advance() {

    pos_->Advance(current_char_);
    if (pos_->idx_ < strlen(source_)) {
        current_char_ = source_[pos_->idx_];
    } else {
        current_char_ = NULL;
    }
}

void CLexer::InitKeywords() {

    v_keywords_ = {
        "crypto",
        "string",
    };
}

void CLexer::MakeEndOfSource() {

    v_tokens_.push_back(new CToken(CToken::ScriptTokenType::END_OF_SOURCE, pos_));

    if (v_tokens_.size() != 1)
        v_tokens_list_.push_back(v_tokens_);

    // // 打印 词法单元
    // PrintTokens();

    v_tokens_.clear();

    Advance();
}

CToken* CLexer::MakeStringOrIdentifier() {

    char str_temp[256];
    memset(str_temp, 0, sizeof(str_temp));

    CToken* Token = NULL;
    char temp[256];
    CPosition* pos_start = pos_->Copy();
    // CPosition *pos_start = NULL;

    while (current_char_ != NULL && IsDigit(current_char_) || current_char_ == '-' ||
           current_char_ == '_' || IsLetter(current_char_)) {

        memset(temp, 0, sizeof(temp));
        sprintf(temp, "%c", current_char_);
        strcat(str_temp, temp);

        Advance();
    }

    if (current_char_ == '=' || (current_char_ == '(' && strlen(str_temp) < 10)) {
        Token = new CToken(CToken::ScriptTokenType::IDENTIFIER, pos_start, pos_, str_temp);
    } else {
        Token = new CToken(CToken::ScriptTokenType::STRING, pos_start, pos_, str_temp);
    }

    // 如果字符串在 v_keywords中，表示它是关键字，否则是变量名
    for (int i = 0; i < v_keywords_.size(); i++) {
        if (strcmp(str_temp, v_keywords_[i].c_str()) == 0) {
            return new CToken(CToken::ScriptTokenType::KEYWORDS, pos_start, pos_, str_temp);
        }
    }

    return Token;
}

CToken* CLexer::MakeString() {

    char str_temp[256];
    memset(str_temp, 0, sizeof(str_temp));

    CToken* Token = NULL;
    char temp[256];
    CPosition* pos_start = pos_->Copy();
    // CPosition *pos_start = NULL;

    while (current_char_ != NULL && IsDigit(current_char_) || IsLetter(current_char_) ||
           current_char_ == '*') {

        memset(temp, 0, sizeof(temp));
        sprintf(temp, "%c", current_char_);
        strcat(str_temp, temp);

        Advance();
    }

    return new CToken(CToken::ScriptTokenType::STRING, pos_start, pos_, str_temp);
}

void CLexer::MakeComments() {

    CPosition* pos_start = pos_->Copy();
    while (current_char_ != '\n') {
        Advance();
    }
}

bool CLexer::IsDigit(char c) { return c >= '0' && c <= '9'; }

bool CLexer::IsLetter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

CLexer::~CLexer() {}