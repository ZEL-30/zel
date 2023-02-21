#include "ast_node.h"

#include <memory>
#include <sstream>

namespace script {

namespace interpreter {

AstNode::AstNode() {}

AstNode::~AstNode() {}

ApduNode::ApduNode(std::shared_ptr<AstNode> apdu, std::vector<std::shared_ptr<AstNode>> v_expects) {
    apdu_ = apdu;
    v_expects_ = v_expects;
}

ApduNode::~ApduNode() {}

std::string ApduNode::str() const {
    std::stringstream ss;
    ss << "(" << apdu_->str() << ", [";
    for (int i = 0; i < v_expects_.size(); i++) {
        if (i != 0)
            ss << ", ";
        ss << v_expects_[i]->str();
    }
    ss << "])";

    return ss.str();
}

std::shared_ptr<AstNode> ApduNode::apdu() { return apdu_; }

BinOpNode::BinOpNode(std::shared_ptr<AstNode> left,
                     std::shared_ptr<Token> bin_op,
                     std::shared_ptr<AstNode> right) {
    left_ = left;
    bin_op_ = bin_op;
    right_ = right;
}

BinOpNode::~BinOpNode() {}

std::string BinOpNode::str() const {
    std::stringstream ss;
    ss << "(" << left_->str() << ", " << bin_op_->str() << ", " << right_->str() << ")";

    return ss.str();
}

std::shared_ptr<AstNode> BinOpNode::left() { return left_; }

std::shared_ptr<Token> BinOpNode::bin_op() { return bin_op_; }

std::shared_ptr<AstNode> BinOpNode::right() { return right_; }

FuncCallNode::FuncCallNode(std::shared_ptr<Token> class_name,
                           std::shared_ptr<AstNode> func_name,
                           std::vector<std::shared_ptr<AstNode>> v_args) {
    class_name_ = class_name;
    func_name_ = func_name;
    v_args_ = v_args;
}

FuncCallNode::~FuncCallNode() {}

std::string FuncCallNode::str() const {
    std::stringstream ss;
    ss << "(" << class_name_->str() << ", " << func_name_->str() << ", [";
    for (int i = 0; i < v_args_.size(); i++) {
        if (i != 0)
            ss << ", ";
        ss << v_args_[i]->str();
    }
    ss << "])";

    return ss.str();
}

std::shared_ptr<Token> FuncCallNode::class_name() { return class_name_; }

std::shared_ptr<AstNode> FuncCallNode::func_name() { return func_name_; }

std::vector<std::shared_ptr<AstNode>> FuncCallNode::func_args() { return v_args_; }

StringNode::StringNode(std::shared_ptr<Token> value) { value_ = value; }

StringNode::~StringNode() {}

std::string StringNode::str() const {
    std::stringstream ss;
    ss << "(" << value_->str() << ")";

    return ss.str();
}

std::string StringNode::value() { return value_->value(); }

VarAccessNode::VarAccessNode(std::shared_ptr<Token> var_name) { var_name_ = var_name; }

VarAccessNode::~VarAccessNode() {}

std::string VarAccessNode::str() const {
    std::stringstream ss;
    ss << "(" << var_name_->str() << ")";

    return ss.str();
}

std::shared_ptr<Token> VarAccessNode::var_name() { return var_name_; }

VarAssignNode::VarAssignNode(std::shared_ptr<Token> var_name, std::shared_ptr<AstNode> value) {
    var_name_ = var_name;
    value_ = value;
}

VarAssignNode::~VarAssignNode() {}

std::string VarAssignNode::str() const {
    std::stringstream ss;
    ss << "(" << var_name_->str() << ", " << value_->str() << ")";

    return ss.str();
};

std::shared_ptr<Token> VarAssignNode::var_name() { return var_name_; }

std::shared_ptr<AstNode> VarAssignNode::value() { return value_; }

} // namespace interpreter
} // namespace zel