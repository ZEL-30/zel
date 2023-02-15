#pragma once
#include "ast_node.h"
#include "lexer.h"
#include "token.h"

#include <memory>
#include <vector>

namespace zel {

namespace interpreter {

// @brief 语法解析器
class Parser {

  public:
    Parser(const std::string& source);

    std::vector<std::shared_ptr<AstNode>> parse();

    ~Parser();

  private:
    std::shared_ptr<AstNode> parseExpr();

    /// @brief 函数调用
    std::shared_ptr<AstNode> parseCall(char* keywords);

    std::shared_ptr<AstNode> parseApdu();

    std::shared_ptr<AstNode> parseTerm();

    std::shared_ptr<AstNode> parseFactor();

    std::shared_ptr<Token> advance();

    std::shared_ptr<Token> rollback();

    std::shared_ptr<AstNode> binOp(std::shared_ptr<AstNode> (Parser::*func_a)(),
                   std::vector<Token::Type>& ops,
                   std::shared_ptr<AstNode> (Parser::*func_b)() = NULL);

  private:
    int token_idx_;
    std::shared_ptr<Token> current_token_;
    std::string source_; // 原文

    std::vector<std::shared_ptr<Token>> v_tokens_;
};

} // namespace interpreter

} // namespace zel