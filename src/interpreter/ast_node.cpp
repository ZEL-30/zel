#include "ast_node.h"

#include <memory>
#include <sstream>


namespace zel {

namespace interpreter {

AstNode::AstNode() {}

AstNode::~AstNode() {}

ApduNode::ApduNode(std::shared_ptr<AstNode> apdu_node,
                   std::vector<std::shared_ptr<AstNode>> v_expect_nodes) {
    apdu_node_ = apdu_node;
    v_expect_nodes_ = v_expect_nodes;
}

ApduNode::~ApduNode() {}

std::string ApduNode::str() const {
    std::stringstream ss;
    ss << "(" << apdu_node_->str() << ", [";
    for (int i = 0; i < v_expect_nodes_.size(); i++) {
        if (i != 0)
            ss << ", ";
        ss << v_expect_nodes_[i]->str();
    }
    ss << "])";

    return ss.str();
}

ApduResultNode::ApduResultNode(std::shared_ptr<Token> apdu_result_token,
                               std::shared_ptr<AstNode> apdu_expect_node) {
    apdu_result_token_ = apdu_result_token;
    apdu_expect_node_ = apdu_expect_node;
}

ApduResultNode::~ApduResultNode() {}

std::string ApduResultNode::str() const {
    std::stringstream ss;
    ss << "(" << apdu_result_token_->str() << ", " << apdu_expect_node_ << ")";

    return ss.str();
}

BinOpNode::BinOpNode(std::shared_ptr<AstNode> left_node,
                     std::shared_ptr<Token> op_token,
                     std::shared_ptr<AstNode> right_node) {
    left_node_ = left_node;
    op_token_ = op_token;
    right_node_ = right_node;
}

BinOpNode::~BinOpNode() {}

std::string BinOpNode::str() const {
    std::stringstream ss;
    ss << "(" << left_node_->str() << ", " << op_token_->str() << ", " << right_node_->str() << ")";

    return ss.str();
}

CryptoClassNode::CryptoClassNode(std::shared_ptr<AstNode> node_to_call,
                                 std::vector<std::shared_ptr<AstNode>> v_arg_nodes) {
    node_to_call_ = node_to_call;
    v_arg_nodes_ = v_arg_nodes;
}

CryptoClassNode::~CryptoClassNode() {}

std::string CryptoClassNode::str() const {
    std::stringstream ss;
    ss << "(" << node_to_call_->str() << ", [";
    for (int i = 0; i < v_arg_nodes_.size(); i++) {
        if (i != 0)
            ss << ", ";
        ss << v_arg_nodes_[i]->str();
    }
    ss << "])";

    return ss.str();
}

StringClassNode::StringClassNode(std::shared_ptr<AstNode> node_to_call,
                                 std::vector<std::shared_ptr<AstNode>> v_arg_nodes) {
    node_to_call_ = node_to_call;
    v_arg_nodes_ = v_arg_nodes;
}

StringClassNode::~StringClassNode() {}

std::string StringClassNode::str() const {
    std::stringstream ss;
    ss << "(" << node_to_call_->str() << ", [";
    for (int i = 0; i < v_arg_nodes_.size(); i++) {
        if (i != 0)
            ss << ", ";
        ss << v_arg_nodes_[i]->str();
    }
    ss << "])";

    return ss.str();
}

StringNode::StringNode(std::shared_ptr<Token> token) { token_ = token; }

StringNode::~StringNode() {}

std::string StringNode::str() const {
    std::stringstream ss;
    ss << "(" << token_->str() << ")";

    return ss.str();
}

VarAccessNode::VarAccessNode(std::shared_ptr<Token> var_name_token) {
    var_name_token_ = var_name_token;
}

VarAccessNode::~VarAccessNode() {}

std::string VarAccessNode::str() const {
    std::stringstream ss;
    ss << "(" << var_name_token_->str() << ")";

    return ss.str();
}

VarAssignNode::VarAssignNode(std::shared_ptr<Token> var_name_token,
                             std::shared_ptr<AstNode> value_node) {
    var_name_token_ = var_name_token;
    value_node_ = value_node;
}

VarAssignNode::~VarAssignNode() {}

std::string VarAssignNode::str() const {
    std::stringstream ss;
    ss << "(" << var_name_token_->str() << ", " << value_node_->str() << ")";

    return ss.str();
};

} // namespace interpreter
} // namespace zel