#include "bin_op_node.h"

CBinOpNode::CBinOpNode(CAstNode* left_node, CToken* op_token, CAstNode* right_node) {
    left_node_ = left_node;
    op_token_ = op_token;
    right_node_ = right_node;

    pos_start_ = left_node_->pos_start_;
    pos_end_ = right_node_->pos_end_;
}

CBinOpNode::~CBinOpNode() {}