#include "ast_node.h"
#include <sstream>



namespace zel {

namespace interpreter {

AstNode::AstNode() {}

AstNode::~AstNode() {}

ApduNode::ApduNode(AstNode* apdu_node, std::vector<AstNode*> v_expect_nodes) {
    apdu_node_ = apdu_node;
    v_expect_nodes_ = v_expect_nodes;

}

ApduNode::~ApduNode() {}

ApduResultNode::ApduResultNode(Token* apdu_result_token, AstNode* apdu_expect_node) {
    apdu_result_token_ = apdu_result_token;
    apdu_expect_node_ = apdu_expect_node;

}

ApduResultNode::~ApduResultNode() {}

BinOpNode::BinOpNode(AstNode* left_node, Token* op_token, AstNode* right_node) {
    left_node_ = left_node;
    op_token_ = op_token;
    right_node_ = right_node;

}

BinOpNode::~BinOpNode() {}


CryptoClassNode::CryptoClassNode(AstNode* node_to_call, std::vector<AstNode*> v_arg_nodes) {
    node_to_call_ = node_to_call;
    v_arg_nodes_ = v_arg_nodes;


}

CryptoClassNode::~CryptoClassNode() {}

StringClassNode::StringClassNode(AstNode* node_to_call, std::vector<AstNode*> v_arg_nodes) {
    node_to_call_ = node_to_call;
    v_arg_nodes_ = v_arg_nodes;


}

StringClassNode::~StringClassNode() {}


StringNode::StringNode(Token* token) {
    token_ = token;
}
std::string StringNode::str() const { 
    std::stringstream ss;
    ss << "(" << token_->value() << ")";

    return ss.str(); 
}


StringNode::~StringNode() {}

VarAccessNode::VarAccessNode(Token* var_name_token) {
    var_name_token_ = var_name_token;

 
}

VarAccessNode::~VarAccessNode() {}

VarAssignNode::VarAssignNode(Token* var_name_token, AstNode* value_node) {
    var_name_token_ = var_name_token;
    value_node_ = value_node;

}

VarAssignNode::~VarAssignNode() {}


std::string VarAssignNode::str() const { 
    std::stringstream ss;

    ss << "(" << var_name_token_->value() << ": " << value_node_->str() << ")";


    return ss.str(); 
};

} // namespace interpreter
} // namespace zel