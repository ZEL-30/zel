#pragma once
#include "ast_node.h"
#include "lexer.h"
#include "token.h"

#include <vector>

namespace zel {

namespace interpreter {

// @brief 语法解析器
class Parser {

  public:
    Parser(const std::string source);

    // std::vector<AstNode*> AstNodesList();

    std::vector<AstNode*> parse();

    ~Parser();

  private:
    AstNode* parseExpr();

    /// @brief 函数调用
    AstNode* parseCall(char* keywords);

    AstNode* parseApdu();

    AstNode* parseTerm();

    AstNode* parseFactor();

    Token* advance();

    Token* rollback();

    AstNode* BinOp(AstNode* (Parser::*func_a)(),
                   std::vector<Token::Type>& ops,
                   AstNode* (Parser::*func_b)() = NULL);

  private:
    int token_idx_;
    Token* current_token_;
    std::string source_; // 原文


    std::vector<Token> v_tokens_;
    std::vector<AstNode*> v_ats_nodes_;
    std::vector<std::vector<Token*>> v_tokens_list_; // 词法单元容器列表
};

} // namespace interpreter

} // namespace zel