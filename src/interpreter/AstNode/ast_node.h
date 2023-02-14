#pragma once
#include "token.h"

/// @brief 抽象语法树 - 节点基类
class CAstNode {

  public:
    CAstNode();

    virtual void Init() = 0;

    ~CAstNode();

  public:
    CToken* token_; // 字符串

    CAstNode* left_node_;
    CToken* op_token_;
    CAstNode* right_node_;

    CToken* var_name_token_;
    CAstNode* value_node_;

    CAstNode* node_to_call_;             // 函数调用对象
    std::vector<CAstNode*> v_arg_nodes_; // 调用函数时传入的参数

    CAstNode* apdu_node_;                   // apdu指令
    std::vector<CAstNode*> v_expect_nodes_; // 期望值容器

    CToken* apdu_result_token_;  // apdu执行结果  ([divbuf]9000) -> divbuf
    CAstNode* apdu_expect_node_; // apdu执行结果  ([divbuf]9000) -> 9000

    CPosition* pos_start_; // 错误起始位置
    CPosition* pos_end_;   // 错误终止位置
};

#include "apdunode/apdu_node.h"
#include "apduresultnode/apdu_result_node.h"
#include "binopnode/bin_op_node.h"
#include "cryptoclassnode/crypto_class_node.h"
#include "stringclassnode/string_class_node.h"
#include "stringnode/string_node.h"
#include "varaccessnode/var_access_node.h"
#include "varassignnode/var_assign_node.h"
