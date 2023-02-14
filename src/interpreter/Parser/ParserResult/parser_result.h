#pragma once

#include "ast_node.h"
#include "illegalcharerror/illegal_char_error.h"
#include "invalidsyntaxerror/invalid_syntax_error.h"

#include <iostream>
#include <typeinfo>


/// @brief 语法解析结果类
class CParserResult {

  public:
    CParserResult();

    CParserResult* Success(CAstNode* node);

    CParserResult* Failure(CError* error);

    int RegisterAdvancement();

    /// @brief 避免递归子层执行报错，父层可以进行判断
    /// @param res
    /// @return
    CAstNode* Register(CParserResult* res);
    CToken* Register(CToken* res);

    ~CParserResult();

  public:
    CError* error_;
    CAstNode* node_;
    std::vector<CAstNode*> v_nodes_;
    int advance_count_;
};
