#pragma once

#include "ast_node.h"

/// @brief 抽象语法树 - 调用算法类型函数节点
class CStringClassNode : public CAstNode {

  public:
    /// @param node_to_call 函数调用对象 ->
    /// @param v_arg_nodes 调用函数时传入的参数
    CStringClassNode(CAstNode* node_to_call, std::vector<CAstNode*> v_arg_nodes);

    virtual void Init() {}

    ~CStringClassNode();
};
