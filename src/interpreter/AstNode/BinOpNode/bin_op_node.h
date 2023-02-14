#pragma once

#include "ast_node.h"

/// @brief 抽象语法树 - 二元操作节点
class CBinOpNode : public CAstNode {

  public:
    /// @brief 二元操作， 例： + - * /
    /// @param left_node  例： 1
    /// @param op_token   例： -
    /// @param right_node 例： 3
    CBinOpNode(CAstNode* left_node, CToken* op_token, CAstNode* right_node);

    virtual void Init() {}

    ~CBinOpNode();
};