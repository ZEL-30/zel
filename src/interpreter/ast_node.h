#pragma once
#include "token.h"

#include <vector>

namespace zel {

namespace interpreter {

/// @brief 抽象语法树 - 节点基类
class AstNode {

  public:
    AstNode();

    virtual std::string str() const = 0;

    ~AstNode();

  public:
    Token* token_; // 字符串

    AstNode* left_node_;
    Token* op_token_;
    AstNode* right_node_;

    Token* var_name_token_;
    AstNode* value_node_;

    AstNode* node_to_call_;             // 函数调用对象
    std::vector<AstNode*> v_arg_nodes_; // 调用函数时传入的参数

    AstNode* apdu_node_;                   // apdu指令
    std::vector<AstNode*> v_expect_nodes_; // 期望值容器

    Token* apdu_result_token_;  // apdu执行结果  ([divbuf]9000) -> divbuf
    AstNode* apdu_expect_node_; // apdu执行结果  ([divbuf]9000) -> 9000
};

/// @brief 抽象语法树 - APDU节点
class ApduNode : public AstNode {

  public:
    /// @param var_name_token apdu指令
    /// @param v_arg_nodes 期望值容器
    ApduNode(AstNode* apdu_node, std::vector<AstNode*> v_expect_nodes);

    virtual std::string str() const { return ""; }

    ~ApduNode();
};

/// @brief 抽象语法树 - 获取APDU执行结果节点
class ApduResultNode : public AstNode {

  public:
    /// @param apdu_result_node apdu执行结果  ([divbuf]9000) -> divbuf
    /// @param apdu_expect_node apdu执行结果  ([divbuf]9000) -> 9000
    ApduResultNode(Token* apdu_result_token, AstNode* apdu_expect_node);

    virtual std::string str() const { return ""; }

    ~ApduResultNode();
};

/// @brief 抽象语法树 - 二元操作节点
class BinOpNode : public AstNode {

  public:
    /// @brief 二元操作， 例： + - * /
    /// @param left_node  例： 1
    /// @param op_token   例： -
    /// @param right_node 例： 3
    BinOpNode(AstNode* left_node, Token* op_token, AstNode* right_node);

    virtual std::string str() const { return ""; }

    ~BinOpNode();
};

/// @brief 抽象语法树 - 调用算法类型函数节点
class CryptoClassNode : public AstNode {

  public:
    /// @param node_to_call 函数调用对象 ->
    /// @param v_arg_nodes 调用函数时传入的参数
    CryptoClassNode(AstNode* node_to_call, std::vector<AstNode*> v_arg_nodes);

    virtual std::string str() const { return ""; }

    ~CryptoClassNode();
};

/// @brief 抽象语法树 - 调用算法类型函数节点
class StringClassNode : public AstNode {

  public:
    /// @param node_to_call 函数调用对象 ->
    /// @param v_arg_nodes 调用函数时传入的参数
    StringClassNode(AstNode* node_to_call, std::vector<AstNode*> v_arg_nodes);

    virtual std::string str() const { return ""; }

    ~StringClassNode();
};

/// @brief 抽象语法树 - 字符串节点
class StringNode : public AstNode {

  public:
    StringNode(Token* token);

    std::string str() const;

    ~StringNode();
};

/// @brief 抽象语法树 - 访问变量节点
class VarAccessNode : public AstNode {

  public:
    VarAccessNode(Token* var_name_token);

    virtual std::string str() const { return ""; }

    ~VarAccessNode();
};

/// @brief 抽象语法树 - 为变量分配值节点
class VarAssignNode : public AstNode {

  public:
    VarAssignNode(Token* var_name_token, AstNode* value_node);

    std::string str() const;

    ~VarAssignNode();
};

} // namespace interpreter

} // namespace zel
