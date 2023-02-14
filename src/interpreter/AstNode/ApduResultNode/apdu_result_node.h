#pragma once

#include "ast_node.h"

/// @brief 抽象语法树 - 获取APDU执行结果节点
class CApduResultNode : public CAstNode {

  public:
    /// @param apdu_result_node apdu执行结果  ([divbuf]9000) -> divbuf
    /// @param apdu_expect_node apdu执行结果  ([divbuf]9000) -> 9000
    CApduResultNode(CToken* apdu_result_token, CAstNode* apdu_expect_node);

    virtual void Init() {}

    ~CApduResultNode();
};