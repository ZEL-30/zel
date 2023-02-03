#include "parser.h"

#include <cctype>
#include <ctype.h>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace zel {

namespace utility {

Parser::Parser() : str_(""), index_(0) {}

Parser::~Parser() {}

bool Parser::LoadFile(const std::string& filename) {

    std::ifstream fin(filename);
    if (fin.fail())
        return false;

    std::stringstream ss;
    ss << fin.rdbuf();
    str_ = ss.str();
    index_ = 0;

    return true;
}

bool Parser::LoadString(const std::string& str) {
    str_ = str;
    index_ = 0;

    return true;
}

Xml Parser::Parse() {

    // 识别声明
    SkipWhiteSpace();
    if (str_.compare(index_, 5, "<?xml") == 0) {
        if (!ParseDeclaration())
            throw std::logic_error("parse declaration error");
    }

    // 识别注释
    SkipWhiteSpace();
    if (str_.compare(index_, 4, "<!--") == 0) {
        if (!ParseComment())
            throw std::logic_error("parse comment error");

        SkipWhiteSpace();
    }
    SkipWhiteSpace();
    while (str_.compare(index_, 4, "<!--") == 0) {
        if (!ParseComment())
            throw std::logic_error("parse comment error");

        SkipWhiteSpace();
    }

    // 识别节点
    if (str_[index_] == '<' &&
        (std::isalpha(str_[index_ + 1]) || str_[index_ + 1] == '_')) {
        return ParseElement();
    }
    throw std::logic_error("parse element error");
}

void Parser::SkipWhiteSpace() {
    while (str_[index_] == ' ' || str_[index_] == '\n' ||
           str_[index_] == '\r' || str_[index_] == '\t') {
        index_++;
    }
}

bool Parser::ParseDeclaration() {

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

bool Parser::ParseComment() {

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

Xml Parser::ParseElement() {

    Xml element;

    index_++;
    SkipWhiteSpace();

    const std::string& name = ParseElementName();
    element.name(name);

    while (str_[index_] != '\0') {
        SkipWhiteSpace();

        if (str_[index_] == '/') {
            if (str_[index_ + 1] == '>') {
                index_ += 2;
            } else {
                throw std::logic_error("xml empty element is error");
            }

        } else if (str_[index_] == '>') {
            index_++;
            std::string text = ParseElementText();
            if (text != "")
                element.text(text);
        } else if (str_[index_] == '<') {
            if (str_[index_ + 1] == '/') {
                // find the end tag
                std::string end_tag = "</" + name + ">";
                size_t pos = str_.find(end_tag, index_);

                // not find the end tag
                if (pos == std::string::npos) {
                    throw std::logic_error("xml element \"" + name +
                                           "\" end tag not found");
                }

                index_ = (pos + end_tag.size());
                break;
            } else if (str_.compare(index_, 4, "<!--") == 0) {
                // 解析注释
                if (!ParseComment())
                    throw std::logic_error("xml comment is error");
            } else {
                // 解析子节点
                element.Append(ParseElement());
            }

        } else {
            // 解析节点属性
            std::string key = ParseElementAttrKey();
            SkipWhiteSpace();

            if (str_[index_] != '=')
                throw std::logic_error("xml element attr is error: " + key);

            index_++;
            SkipWhiteSpace();

            std::string value = ParseElementAttrValue();
            element.attr(key, value);

            break;
        }
    }

    return element;
}

std::string Parser::ParseElementName() {

    int pos = index_;
    if (isalpha(str_[index_]) || str_[index_] == '_') {
        index_++;
        while (isalnum(str_[index_]) || (str_[index_] == ':') ||
               (str_[index_] == '-') || (str_[index_] == '.')) {
            index_++;
        }
    }

    return str_.substr(pos, index_ - pos);
}

std::string Parser::ParseElementText() {

    int pos = index_;
    while (str_[index_] != '<') {
        index_++;
    }

    return str_.substr(pos, index_ - pos);
}

std::string Parser::ParseElementAttrKey() {

    int pos = index_;
    if (isalpha(str_[index_]) || (str_[index_] == '_')) {
        index_++;
        while (isalnum(str_[index_]) || (str_[index_] == '_') ||
               (str_[index_] == '-') || (str_[index_] == ':')) {
            index_++;
        }
    }

    return str_.substr(pos, index_ - pos);
}

std::string Parser::ParseElementAttrValue() {

    // 判断属性值是否以双引号开始
    if (str_[index_] != '"')
        throw std::logic_error(
            "xml element attr value should be in double quotes");

    int pos = ++index_;
    while (str_[index_] != '"') {
        index_++;
    }
    index_++;

    return str_.substr(pos, index_ - pos - 1);
}

} // namespace utility

} // namespace zel