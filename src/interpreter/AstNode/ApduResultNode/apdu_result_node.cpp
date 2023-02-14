#include "apdu_result_node.h"

CApduResultNode::CApduResultNode(CToken* apdu_result_token, CAstNode* apdu_expect_node) {
    apdu_result_token_ = apdu_result_token;
    apdu_expect_node_ = apdu_expect_node;

    pos_start_ = apdu_result_token_->pos_start_;
    pos_end_ = apdu_expect_node_->pos_end_;
}

CApduResultNode::~CApduResultNode() {}