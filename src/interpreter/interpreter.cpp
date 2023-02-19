#include "interpreter.h"

#include <memory>

namespace zel {
namespace interpreter {

Interpreter::Interpreter() { getAstType(); }

Interpreter::~Interpreter() {}

std::shared_ptr<Value> Interpreter::visit(std::shared_ptr<AstNode> node) {

    switch (m_ast_node_type_[typeid(*node).name()]) {

    case String:
        return visitString(node);

    case BinOp:
        return visitBinOp(node);

    case VarAccess:
        return visitVarAccess(node);

    case VarAssign:
        return visitVarAssign(node);

    case Apdu:
        return visitApdu(node);

    case FuncCall:
        return visitFuncCall(node);

    default:
        break;
    }
}

std::shared_ptr<Value> Interpreter::visitString(std::shared_ptr<AstNode> node) {
    std::shared_ptr<StringNode> string_node = std::dynamic_pointer_cast<StringNode>(node);
    return std::make_unique<Value>(string_node->value());
}

std::shared_ptr<Value> Interpreter::visitBinOp(std::shared_ptr<AstNode> node) {

    std::shared_ptr<BinOpNode> bin_op_node = std::dynamic_pointer_cast<BinOpNode>(node);

    // 左递归
    auto left = visit(bin_op_node->left());

    // 右递归
    auto right = visit(bin_op_node->right());

    std::shared_ptr<Value> value = nullptr;
    switch (bin_op_node->bin_op()->type()) {
    case Token::PLUS:
        value = left->append(right);
        break;

    default:
        break;
    }

    return value;
}

std::shared_ptr<Value> Interpreter::visitVarAccess(std::shared_ptr<AstNode> node) {
    return nullptr;
}

std::shared_ptr<Value> Interpreter::visitVarAssign(std::shared_ptr<AstNode> node) {
    return nullptr;
}

std::shared_ptr<Value> Interpreter::visitApdu(std::shared_ptr<AstNode> node) { return nullptr; }

std::shared_ptr<Value> Interpreter::visitFuncCall(std::shared_ptr<AstNode> node) { return nullptr; }

void Interpreter::getAstType() {
    m_ast_node_type_["10StringNode"] = String;
    m_ast_node_type_["9BinOpNode"] = BinOp;
    m_ast_node_type_["13VarAccessNode"] = VarAccess;
    m_ast_node_type_["13VarAssignNode"] = VarAssign;
    m_ast_node_type_["12FuncCallNode"] = FuncCall;
    m_ast_node_type_["8ApduNode"] = Apdu;
}

} // namespace interpreter

} // namespace zel