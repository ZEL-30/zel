#pragma once

#include "ast_node.h"

/// @brief 抽象语法树 - 为变量分配值节点
class CVarAssignNode : public CAstNode {

  public:
    CVarAssignNode(CToken* var_name_token, CAstNode* value_node);

    virtual void Init(){};

    ~CVarAssignNode();
};
