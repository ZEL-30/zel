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

FuncCallNode::FuncCallNode(std::shared_ptr<Token> class_name,
                           std::shared_ptr<AstNode> func_name,
                           std::vector<std::shared_ptr<AstNode>> v_arg_nodes) {
    class_name_ = class_name;
    func_name_ = func_name;
    v_arg_nodes_ = v_arg_nodes;
}

FuncCallNode::~FuncCallNode() {}

std::string FuncCallNode::str() const {
    std::stringstream ss;
    ss << "(" << class_name_->str() << ", " << func_name_->str() << ", [";
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