#pragma once
#include "token.h"

#include <memory>
#include <vector>

namespace zel {

namespace interpreter {

/// @brief 抽象语法树 - 节点基类
class AstNode {

  public:
    AstNode();
    ~AstNode();

    virtual std::string str() const = 0;

    std::shared_ptr<Token> var_name_token_;
};

/// @brief 抽象语法树 - APDU节点
class ApduNode : public AstNode {

  public:
    /// @param var_name_token apdu指令
    /// @param v_arg_nodes 期望值容器
    ApduNode(std::shared_ptr<AstNode> apdu_node,
             std::vector<std::shared_ptr<AstNode>> v_expect_nodes);

    std::string str() const;

    ~ApduNode();

  private:
    std::shared_ptr<AstNode> apdu_node_;                   // apdu指令
    std::vector<std::shared_ptr<AstNode>> v_expect_nodes_; // 期望值容器
};

/// @brief 抽象语法树 - 二元操作节点
class BinOpNode : public AstNode {

  public:
    /// @brief 二元操作， 例： + - * /
    /// @param left_node  例： 1
    /// @param op_token   例： -
    /// @param right_node 例： 3
    BinOpNode(std::shared_ptr<AstNode> left_node,
              std::shared_ptr<Token> op_token,
              std::shared_ptr<AstNode> right_node);
    ~BinOpNode();

    std::string str() const;

  private:
    std::shared_ptr<AstNode> left_node_;
    std::shared_ptr<Token> op_token_;
    std::shared_ptr<AstNode> right_node_;
};

/// @brief 抽象语法树 - 调用算法类型函数节点
class FuncCallNode : public AstNode {

  public:
    /// @param class_name 类名
    /// @param func_name  函数名
    /// @param v_arg_nodes 调用函数时传入的参数
    FuncCallNode(std::shared_ptr<Token> class_name,
                 std::shared_ptr<AstNode> func_name,
                 std::vector<std::shared_ptr<AstNode>> v_arg_nodes);
    ~FuncCallNode();

    std::string str() const;

  private:
    std::shared_ptr<Token> class_name_;                 // 类名
    std::shared_ptr<AstNode> func_name_;                // 函数名
    std::vector<std::shared_ptr<AstNode>> v_arg_nodes_; // 调用函数时传入的参数
};

/// @brief 抽象语法树 - 字符串节点
class StringNode : public AstNode {

  public:
    StringNode(std::shared_ptr<Token> token);
    ~StringNode();

    std::string str() const;

  private:
    std::shared_ptr<Token> token_; // 字符串
};

/// @brief 抽象语法树 - 访问变量节点
class VarAccessNode : public AstNode {

  public:
    VarAccessNode(std::shared_ptr<Token> var_name_token);
    ~VarAccessNode();

    std::string str() const;

  private:
    std::shared_ptr<Token> var_name_token_;
};

/// @brief 抽象语法树 - 为变量分配值节点
class VarAssignNode : public AstNode {

  public:
    VarAssignNode(std::shared_ptr<Token> var_name_token, std::shared_ptr<AstNode> value_node);
    ~VarAssignNode();

    std::string str() const;

  private:
    std::shared_ptr<Token> var_name_token_;
    std::shared_ptr<AstNode> value_node_;
};

} // namespace interpreter

} // namespace zel
