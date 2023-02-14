#pragma once

#include "ast_node.h"

/// @brief 抽象语法树 - 字符串节点
class CStringNode : public CAstNode {

  public:
    CStringNode(CToken* token);

    virtual void Init() {}

    ~CStringNode();
};