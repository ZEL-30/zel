#include "apdu_node.h"

CApduNode::CApduNode(CAstNode* apdu_node, std::vector<CAstNode*> v_expect_nodes) {
    apdu_node_ = apdu_node;
    v_expect_nodes_ = v_expect_nodes;

    pos_start_ = apdu_node_->pos_start_;

    // 调用函数， 传参时
    if (v_expect_nodes_.size() > 0) {
        pos_end_ = v_expect_nodes_[v_expect_nodes_.size() - 1]->pos_end_;

        // 调用函数， 不传参时
    } else {
        pos_end_ = apdu_node_->pos_end_;
    }
}

CApduNode::~CApduNode() {}