#pragma once

#include "ast_node.h"
#include "symbol_table.h"
#include "value.h"

#include <map>
#include <memory>
#include <string>

namespace script {

namespace interpreter {

/// @brief  解释器
class Interpreter {

  public:
    Interpreter();
    ~Interpreter();

    /// @brief 递归下降算法 -> ast node
    /// @param node 起始节点
    std::shared_ptr<Value> visit(std::shared_ptr<AstNode> node);

  private:
    std::shared_ptr<Value> visitString(std::shared_ptr<AstNode> node);
    std::shared_ptr<Value> visitBinOp(std::shared_ptr<AstNode> node);
    std::shared_ptr<Value> visitVarAccess(std::shared_ptr<AstNode> node);
    std::shared_ptr<Value> visitVarAssign(std::shared_ptr<AstNode> node);
    std::shared_ptr<Value> visitApdu(std::shared_ptr<AstNode> node);
    std::shared_ptr<Value> visitFuncCall(std::shared_ptr<AstNode> node);

    void initSymbolTable();

    void getAstType();

  private:

    std::unique_ptr<SymbolTable> symbol_table_;

    enum AstType { String, BinOp, VarAccess, VarAssign, FuncCall, Apdu };
    std::map<std::string, AstType> m_ast_node_type_;
};

} // namespace interpreter

} // namespace zel
