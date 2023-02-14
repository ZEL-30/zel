#pragma once

#include "error.h"
#include "token.h"

#include <stdlib.h>
#include <vector>

/// @brief 词法分析器
class CLexer {

  public:
    CLexer(char* file_name, char* source);

    /// @brief 将原文转换成词法单元
    /// @return 错误类
    std::vector<std::vector<CToken*>> MakeTokensList();

    /// @brief 打印词法单元流
    void PrintTokens();

    ~CLexer();

  private:
    /// @brief 初始化关键字容器
    void InitKeywords();

    /// @brief 预读
    void Advance();

    /// @brief 匹配字符串 或者 标识符
    CToken* MakeStringOrIdentifier();

    /// @brief 匹配字符串
    CToken* MakeString();

    /// @brief 匹配注释
    void MakeComments();

    /// @brief  匹配 '\n'
    void MakeEndOfSource();

    /// @brief 判断字符是否是数字
    /// @param c 待判断的字符
    bool IsDigit(char c);

    /// @brief 判断字符是否是字母
    /// @param c 待判断的字符
    bool IsLetter(char c);

  public:
    CPosition* pos_; // 位置

  private:
    char source_[1024000]; // 原文
    char file_name_[1024]; // 原文文件名
    char current_char_;    // 当前的字符
    // CPosition* pos_;       // 位置

    std::vector<std::string> v_keywords_;             // 关键字容器
    std::vector<CToken*> v_tokens_;                   // 词法单元容器
    std::vector<std::vector<CToken*>> v_tokens_list_; // 词法单元容器列表
};
