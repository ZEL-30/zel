#include "crypto_class_node.h"

CCryptoClassNode::CCryptoClassNode(CAstNode* node_to_call, std::vector<CAstNode*> v_arg_nodes) {
    node_to_call_ = node_to_call;
    v_arg_nodes_ = v_arg_nodes;

    pos_start_ = node_to_call_->pos_start_;

    // 调用函数， 传参时
    if (v_arg_nodes_.size() > 0) {
        pos_end_ = v_arg_nodes_[v_arg_nodes_.size() - 1]->pos_end_;

        // 调用函数， 不传参时
    } else {
        pos_end_ = node_to_call_->pos_end_;
    }
}

CCryptoClassNode::~CCryptoClassNode() {}