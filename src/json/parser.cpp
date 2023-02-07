/// @file parser.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-02-07
/// @copyright Copyright (c) 2023 ZEL

#include "parser.h"

#include "json.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace zel {
namespace json {

Parser::Parser() : str_(""), index_(0) {}

Parser::~Parser() {}

bool Parser::loadFile(const std::string& filename) {

    std::ifstream fin(filename);
    if (fin.fail())
        return false;

    std::stringstream ss;
    ss << fin.rdbuf();
    str_ = ss.str();
    index_ = 0;

    return true;
}

bool Parser::loadString(const std::string& str) {
    str_ = str;
    index_ = 0;

    return true;
}

Json Parser::parse() {

    char ch = getNextChar();

    switch (ch) {
    case 'n':
        return parseNull();

    case 't':
    case 'f':
        return parseBool();

    case '-':
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
        return parseNumber();

    case '"':
        return Json(parseString());

    case '[':
        return parseArray();

    case '{':
        return parseObject();

    default:
        throw std::logic_error("unexpected char '" + std::string(1, ch) + "'");
        break;
    }
}

void Parser::skipWhiteSpace() {

    while (str_[index_] == ' ' || str_[index_] == '\n' || str_[index_] == '\t' ||
           str_[index_] == '\r') {
        index_++;
    }
}

char Parser::getNextChar() {
    skipWhiteSpace();
    return str_[index_++];
}

Json Parser::parseNull() {
    index_--;
    if (str_.compare(index_, 4, "null") == 0) {
        index_ += 4;
        return Json();
    }

    throw std::logic_error("parse null error");
}

Json Parser::parseBool() {

    index_--;
    if (str_.compare(index_, 4, "true") == 0) {
        index_ += 4;
        return Json(true);
    } else if (str_.compare(index_, 5, "false") == 0) {
        index_ += 5;
        return Json(false);
    }

    throw std::logic_error("parse bool error");
}

Json Parser::parseNumber() {

    index_--;

    int pos = index_;

    if (str_[index_] == '-')
        index_++;

    if (str_[index_] < '0' || str_[index_] > '9')
        throw std::logic_error("parse number error");

    while (str_[index_] >= '0' && str_[index_] <= '9') {
        index_++;
    }

    // 120
    if (str_[index_] != '.') {
        return Json(std::stoi(str_.substr(pos, index_ - pos)));
    }

    index_++;

    if (str_[index_] < '0' || str_[index_] > '9')
        throw std::logic_error("parse number error");

    while (str_[index_] >= '0' && str_[index_] <= '9') {
        index_++;
    }

    return Json(std::stof(str_.substr(pos, index_ - pos)));
}

std::string Parser::parseString() {

    std::string out;

    char ch;
    while (true) {
        ch = str_[index_++];

        if (ch == '"') {
            break;
        }

        if (ch == '\\') {

            ch = str_[index_++];

            switch (ch) {

            case '\n':
                out += '\n';
                break;

            case '\r':
                out += '\r';
                break;

            case '\t':
                out += '\t';
                break;

            case '\b':
                out += '\b';
                break;
            case '\f':
                out += '\f';
                break;
            case '"':
                out += "\\\"";
                break;
            case '\\':
                out += "\\\\";
                break;

            case 'u':
                out += "\\u";
                for (int i = 0; i < 4; i++) {
                    out += str_[index_++];
                }
                break;

            default:
                break;
            }

        } else {
            out += ch;
        }
    }

    return out;
}

Json Parser::parseArray() {

    Json arr(Json::JSON_ARRAY);

    char ch = getNextChar();

    if (ch == ']') {
        return arr;
    }
    index_--;
    while (true) {
        arr.append(parse());

        ch = getNextChar();
        if (ch == ']')
            break;

        if (ch != ',')
            throw std::logic_error("parse array error");

        skipWhiteSpace();
    }

    return arr;
}

Json Parser::parseObject() {

    Json object(Json::JSON_OBJECT);

    char ch = getNextChar();

    if (ch == '}')
        return object;

    index_--;
    while (true) {
        // "
        ch = getNextChar();
        if (ch != '"')
            throw std::logic_error("parse object error");

        // key
        std::string key = parseString();

        // :
        ch = getNextChar();
        if (ch != ':')
            throw std::logic_error("parse object error");

        // value
        object[key] = parse();

        // }
        ch = getNextChar();
        if (ch == '}')
            break;

        // ,
        if (ch != ',')
            throw std::logic_error("parse object error");

        skipWhiteSpace();
    }

    return object;
}

} // namespace json
} // namespace zel