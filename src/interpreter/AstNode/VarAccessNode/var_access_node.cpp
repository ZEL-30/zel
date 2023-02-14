#include "var_access_node.h"

CVarAccessNode::CVarAccessNode(CToken* var_name_token) {
    var_name_token_ = var_name_token;

    pos_start_ = var_name_token_->pos_start_;
    pos_end_ = var_name_token_->pos_end_;
}

CVarAccessNode::~CVarAccessNode() {}