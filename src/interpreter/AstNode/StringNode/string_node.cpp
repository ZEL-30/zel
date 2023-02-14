#include "string_node.h"

CStringNode::CStringNode(CToken* token) {
    token_ = token;
    pos_start_ = token_->pos_start_;
    pos_end_ = token_->pos_end_;
}

CStringNode::~CStringNode() {}
