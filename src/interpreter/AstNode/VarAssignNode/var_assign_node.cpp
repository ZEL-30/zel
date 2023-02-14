#include "var_assign_node.h"

CVarAssignNode::CVarAssignNode(CToken* var_name_token, CAstNode* value_node) {
    var_name_token_ = var_name_token;
    value_node_ = value_node;

    pos_start_ = var_name_token_->pos_start_;
    pos_end_ = var_name_token_->pos_end_;
}

CVarAssignNode::~CVarAssignNode() {}
