#pragma once

#include "xml.h"

#include <iostream>

namespace zel {

namespace xml {

class Parser {

  public:
    Parser();
    ~Parser();

    /// @brief 加载文件
    /// @param file 文件名
    bool loadFile(const std::string& filename);

    /// @brief 加载字符串
    /// @param str 待加载的字符串
    bool loadString(const std::string& str);

    /// @brief 解析XML文件
    /// @return Xml XML根节点
    Xml parse();

  private:
    /// @brief 跳过空格
    void skipWhiteSpace();

    /// @brief 解析声明
    bool parseDeclaration();

    /// @brief 解析注释
    bool parseComment();

    /// @brief 解析元素
    /// @return Xml
    Xml parseElement();

    /// @brief 解析元素名称
    /// @return std::string 元素名称
    std::string parseElementName();

    /// @brief 解析元素内容
    /// @return std::string 元素内容
    std::string parseElementText();

    /// @brief 解析元素属性键
    /// @return std::string 元素属性键
    std::string parseElementAttrKey();

    /// @brief 解析元素属性值
    /// @return std::string 元素属性值
    std::string parseElementAttrValue();

  private:
    std::string str_; // xml buffer
    int index_;       // 下标
};

} // namespace xml

} // namespace zel