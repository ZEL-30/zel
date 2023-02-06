#pragma once

#include "xml/xml.h"

#include <iostream>

namespace zel {

namespace xml {

class Parser {

  public:
    Parser();
    ~Parser();

    /// @brief 加载文件
    /// @param file 文件名
    bool LoadFile(const std::string& filename);

    /// @brief 加载字符串
    /// @param str 待加载的字符串
    bool LoadString(const std::string& str);

    /// @brief 解析XML文件
    /// @return Xml XML根节点
    Xml Parse();

  private:
    /// @brief 跳过空格
    void SkipWhiteSpace();

    /// @brief 解析声明
    bool ParseDeclaration();

    /// @brief 解析注释
    bool ParseComment();

    /// @brief 解析元素
    /// @return Xml
    Xml ParseElement();

    /// @brief 解析元素名称
    /// @return std::string 元素名称
    std::string ParseElementName();

    /// @brief 解析元素内容
    /// @return std::string 元素内容
    std::string ParseElementText();

    /// @brief 解析元素属性键
    /// @return std::string 元素属性键
    std::string ParseElementAttrKey();

    /// @brief 解析元素属性值
    /// @return std::string 元素属性值
    std::string ParseElementAttrValue();

  private:
    std::string str_; // xml buffer
    int index_;       // 下标
};

} // namespace utility

} // namespace zel