#pragma once
#include "token.h"

#include <memory>
#include <vector>

namespace script {

namespace interpreter {

/// @brief 抽象语法树 - 节点基类
class AstNode {

  public:
    AstNode();
    ~AstNode();

    virtual std::string str() const = 0;

  private:
};

/// @brief 抽象语法树 - APDU节点
class ApduNode : public AstNode {

  public:
    /// @param var_name apdu指令
    /// @param v_args 期望值容器
    ApduNode(std::shared_ptr<AstNode> apdu, std::vector<std::shared_ptr<AstNode>> v_expects);
    ~ApduNode();

    std::string str() const;

    std::shared_ptr<AstNode> apdu();

  private:
    std::shared_ptr<AstNode> apdu_;                   // apdu指令
    std::vector<std::shared_ptr<AstNode>> v_expects_; // 期望值容器
};

/// @brief 抽象语法树 - 二元操作节点
class BinOpNode : public AstNode {

  public:
    /// @brief 二元操作， 例： + - * /
    /// @param left  例： 1
    /// @param op   例： -
    /// @param right 例： 3
    BinOpNode(std::shared_ptr<AstNode> left,
              std::shared_ptr<Token> op,
              std::shared_ptr<AstNode> right);
    ~BinOpNode();

    std::string str() const;

    std::shared_ptr<AstNode> left();
    std::shared_ptr<Token> bin_op();
    std::shared_ptr<AstNode> right();

  private:
    std::shared_ptr<AstNode> left_;
    std::shared_ptr<Token> bin_op_;
    std::shared_ptr<AstNode> right_;
};

/// @brief 抽象语法树 - 调用算法类型函数节点
class FuncCallNode : public AstNode {

  public:
    /// @param class_name 类名
    /// @param func_name  函数名
    /// @param v_args 调用函数时传入的参数
    FuncCallNode(std::shared_ptr<Token> class_name,
                 std::shared_ptr<AstNode> func_name,
                 std::vector<std::shared_ptr<AstNode>> v_args);
    ~FuncCallNode();

    std::string str() const;

    std::shared_ptr<Token> class_name();
    std::shared_ptr<AstNode> func_name();
    std::vector<std::shared_ptr<AstNode>> func_args();

  private:
    std::shared_ptr<Token> class_name_;            // 类名
    std::shared_ptr<AstNode> func_name_;           // 函数名
    std::vector<std::shared_ptr<AstNode>> v_args_; // 调用函数时传入的参数
};

/// @brief 抽象语法树 - 字符串节点
class StringNode : public AstNode {

  public:
    StringNode(std::shared_ptr<Token> token);
    ~StringNode();

    std::string str() const;

    std::string value();

  private:
    std::shared_ptr<Token> value_; // 字符串
};

/// @brief 抽象语法树 - 访问变量节点
class VarAccessNode : public AstNode {

  public:
    VarAccessNode(std::shared_ptr<Token> var_name);
    ~VarAccessNode();

    std::string str() const;

    std::shared_ptr<Token> var_name();

  private:
    std::shared_ptr<Token> var_name_;
};

/// @brief 抽象语法树 - 为变量分配值节点
class VarAssignNode : public AstNode {

  public:
    VarAssignNode(std::shared_ptr<Token> var_name, std::shared_ptr<AstNode> value);
    ~VarAssignNode();

    std::string str() const;

    std::shared_ptr<Token> var_name();
    std::shared_ptr<AstNode> value();

  private:
    std::shared_ptr<Token> var_name_;
    std::shared_ptr<AstNode> value_;
};

} // namespace interpreter

} // namespace zel
