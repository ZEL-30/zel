#pragma once

#include "ast_node.h"

/// @brief 抽象语法树 - APDU节点
class CApduNode : public CAstNode {

  public:
    /// @param var_name_token apdu指令
    /// @param v_arg_nodes 期望值容器
    CApduNode(CAstNode* apdu_node, std::vector<CAstNode*> v_expect_nodes);

    virtual void Init() {}

    ~CApduNode();
};