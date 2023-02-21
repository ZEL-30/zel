#include "interpreter.h"

#include <memory>

namespace script {
namespace interpreter {

Interpreter::Interpreter() {
    getAstType();

    initSymbolTable();
}

Interpreter::~Interpreter() {}

std::shared_ptr<Value> Interpreter::visit(std::shared_ptr<AstNode> node) {
    // printf("%s\n", typeid(*node).name());
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
    auto string_node = std::dynamic_pointer_cast<StringNode>(node);
    return std::make_unique<Value>(string_node->value());
}

std::shared_ptr<Value> Interpreter::visitBinOp(std::shared_ptr<AstNode> node) {
    auto bin_op_node = std::dynamic_pointer_cast<BinOpNode>(node);

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
        char error[256];
        memset(error, 0, sizeof(error));
        sprintf(error, "%s is not suppert", bin_op_node->bin_op()->str().c_str());
        printf("%s\n", error);
        break;
    }

    return value;
}

std::shared_ptr<Value> Interpreter::visitVarAccess(std::shared_ptr<AstNode> node) {
    auto var_access = std::dynamic_pointer_cast<VarAccessNode>(node);

    // 从token中获得变量名
    std::string var_name = var_access->var_name()->value();

    // 从符号表中取值
    auto value = symbol_table_->symbolTable(var_name);

    if (value == nullptr) {
        char error[1024];
        memset(error, 0, sizeof(error));
        sprintf(error, "%s is not defined", var_name.c_str());
        printf("%s\n", error);
    }

    return value;
}

std::shared_ptr<Value> Interpreter::visitVarAssign(std::shared_ptr<AstNode> node) {
    auto var_assign = std::dynamic_pointer_cast<VarAssignNode>(node);
    // 从token中获得变量名
    std::string var_name = var_assign->var_name()->value();

    auto value = visit(var_assign->value());

    symbol_table_->symbolTable(var_name, value);

    return value;
}

std::shared_ptr<Value> Interpreter::visitApdu(std::shared_ptr<AstNode> node) {
    // auto apdu = std::dynamic_pointer_cast<StringNode>(
    //     visit(std::dynamic_pointer_cast<ApduNode>(node)->apdu()));

    // std::string str_apdu = apdu->value();

    // printf("%s\n", str_apdu.c_str());
    // // 执行APDU指令
    // CApdu Apdu(str_apdu, ReaderFac_);
    // CValue* apdu_result = Apdu.Execute();

    // // 将执行结果添加到符号表
    // context->symbol_table_->Set("apdu_result", apdu_result);

    // std::vector<CValue*> v_expected;
    // for (int i = 0; i < node->v_expect_nodes_.size(); i++) {
    //     v_expected.push_back(res->Register(Visit(node->v_expect_nodes_[i], context)));
    // }

    // // 比对 相应值 和 期望值
    // CValue* Value = Apdu.Compare(v_expected);
    // Value->SetPos(node->pos_start_, node->pos_end_);

    return nullptr;
}

std::shared_ptr<Value> Interpreter::visitFuncCall(std::shared_ptr<AstNode> node) {

    std::vector<std::shared_ptr<Value>> v_args;

    // VisitAccessNode()方法通过节点名（即函数名）， 从符号表中还原函数对象本身
    auto func_call = std::dynamic_pointer_cast<FuncCallNode>(node);
    auto func_name = visit(func_call->func_name());

    printf("函数名: %s\n", func_name->str().c_str());
    for (int i = 0; i < func_call->func_args().size(); i++) {
        v_args.push_back(visit(func_call->func_args()[i]));
        printf("参数%d: %s\n", i, visit(func_call->func_args()[i])->str().c_str());
    }


    return nullptr;
}

void Interpreter::initSymbolTable() {
    symbol_table_ = std::make_unique<SymbolTable>();

    symbol_table_->symbolTable("mid", std::make_shared<Value>("mid"));

}

void Interpreter::getAstType() {
    m_ast_node_type_["class script::interpreter::StringNode"] = String;
    m_ast_node_type_["class script::interpreter::BinOpNode"] = BinOp;
    m_ast_node_type_["class script::interpreter::VarAccessNode"] = VarAccess;
    m_ast_node_type_["class script::interpreter::VarAssignNode"] = VarAssign;
    m_ast_node_type_["class script::interpreter::FuncCallNode"] = FuncCall;
    m_ast_node_type_["class script::interpreter::ApduNode"] = Apdu;
}

} // namespace interpreter

} // namespace zel