/// @file parser.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-02-07
/// @copyright Copyright (c) 2023 ZEL

#include "parser.h"

#include <cctype>
#include <ctype.h>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace zel {

namespace xml {

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

Xml Parser::parse() {

    // 识别声明
    skipWhiteSpace();
    if (str_.compare(index_, 5, "<?xml") == 0) {
        if (!parseDeclaration())
            throw std::logic_error("parse declaration error");
    }

    // 识别注释
    skipWhiteSpace();
    if (str_.compare(index_, 4, "<!--") == 0) {
        if (!parseComment())
            throw std::logic_error("parse comment error");

        skipWhiteSpace();
    }
    skipWhiteSpace();
    while (str_.compare(index_, 4, "<!--") == 0) {
        if (!parseComment())
            throw std::logic_error("parse comment error");

        skipWhiteSpace();
    }

    // 识别节点
    if (str_[index_] == '<' && (std::isalpha(str_[index_ + 1]) || str_[index_ + 1] == '_')) {
        return parseElement();
    }
    throw std::logic_error("parse element error");
}

void Parser::skipWhiteSpace() {
    while (str_[index_] == ' ' || str_[index_] == '\n' || str_[index_] == '\r' ||
           str_[index_] == '\t') {
        index_++;
    }
}

bool Parser::parseDeclaration() {

    if (str_.compare(index_, 5, "<?xml") != 0)
        return false;

    index_ += 5;
    size_t pos = str_.find("?>", index_);

    // 如果没有找到 "？>"
    if (pos == std::string::npos)
        return false;

    index_ = pos + 2;

    return true;
}

bool Parser::parseComment() {

    if (str_.compare(index_, 4, "<!--") != 0)
        return false;

    index_ += 4;

    size_t pos = str_.find("-->", index_);

    // 如果没有找到 "-->"
    if (pos == std::string::npos)
        return false;

    index_ = pos + 3;

    return true;
}

Xml Parser::parseElement() {

    Xml element;

    index_++;
    skipWhiteSpace();

    const std::string& name = parseElementName();
    element.name(name);

    while (str_[index_] != '\0') {
        skipWhiteSpace();

        if (str_[index_] == '/') {
            if (str_[index_ + 1] == '>') {
                index_ += 2;
                break;
            } else {
                throw std::logic_error("xml empty element is error");
            }

        } else if (str_[index_] == '>') {
            index_++;
            std::string text = parseElementText();
            if (text != "")
                element.text(text);
        } else if (str_[index_] == '<') {
            if (str_[index_ + 1] == '/') {
                // find the end tag
                std::string end_tag = "</" + name + ">";
                size_t pos = str_.find(end_tag, index_);

                // not find the end tag
                if (pos == std::string::npos) {
                    throw std::logic_error("xml element \"" + name + "\" end tag not found");
                }

                index_ = (pos + end_tag.size());
                break;
            } else if (str_.compare(index_, 4, "<!--") == 0) {
                // 解析注释
                if (!parseComment())
                    throw std::logic_error("xml comment is error");
            } else {
                // 解析子节点
                element.append(parseElement());
            }

        } else {
            // 解析节点属性
            std::string key = parseElementAttrKey();
            skipWhiteSpace();

            if (str_[index_] != '=')
                throw std::logic_error("xml element attr is error: " + key);

            index_++;
            skipWhiteSpace();

            std::string value = parseElementAttrValue();
            element.attr(key, value);
        }
    }

    return element;
}

std::string Parser::parseElementName() {

    int pos = index_;
    if (isalpha(str_[index_]) || str_[index_] == '_') {
        index_++;
        while (isalnum(str_[index_]) || (str_[index_] == ':') || (str_[index_] == '-') ||
               (str_[index_] == '.')) {
            index_++;
        }
    }

    return str_.substr(pos, index_ - pos);
}

std::string Parser::parseElementText() {

    int pos = index_;
    while (str_[index_] != '<') {
        index_++;
    }

    return str_.substr(pos, index_ - pos);
}

std::string Parser::parseElementAttrKey() {

    int pos = index_;
    if (isalpha(str_[index_]) || (str_[index_] == '_')) {
        index_++;
        while (isalnum(str_[index_]) || (str_[index_] == '_') || (str_[index_] == '-') ||
               (str_[index_] == ':')) {
            index_++;
        }
    }

    return str_.substr(pos, index_ - pos);
}

std::string Parser::parseElementAttrValue() {

    // 判断属性值是否以双引号开始
    if (str_[index_] != '"')
        throw std::logic_error("xml element attr value should be in double quotes");

    int pos = ++index_;
    while (str_[index_] != '"') {
        index_++;
    }
    index_++;

    return str_.substr(pos, index_ - pos - 1);
}

} // namespace xml

} // namespace zel