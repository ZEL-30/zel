#pragma once

#include "ast_node.h"

/// @brief 抽象语法树 - 访问变量节点
class CVarAccessNode : public CAstNode {

  public:
    CVarAccessNode(CToken* var_name_token);

    virtual void Init() {}

    ~CVarAccessNode();
};
