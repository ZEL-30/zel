#include "interpreter.h"

#include "interpreter/ast_node.h"
#include "interpreter/value.h"

#include <memory>

namespace zel {

namespace interpreter {

Interpreter::Interpreter() { getAstType(); }

Interpreter::~Interpreter() {}

Value Interpreter::visit(std::shared_ptr<AstNode> node) {

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

    return Value();
}

void Interpreter::getAstType() {
    m_ast_node_type_["11CStringNode"] = String;
    m_ast_node_type_["10CBinOpNode"] = BinOp;
    m_ast_node_type_["14CVarAccessNode"] = VarAccess;
    m_ast_node_type_["14CVarAssignNode"] = VarAssign;
    m_ast_node_type_["9CCallNode"] = FuncCall;
    m_ast_node_type_["9CApduNode"] = Apdu;
}

} // namespace interpreter

} // namespace zel