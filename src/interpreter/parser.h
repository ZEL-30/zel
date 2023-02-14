// #pragma once
// #include "ast_node.h"
// #include "lexer.h"
// #include "token.h"

// #include <vector>

// namespace zel {

// namespace interpreter {

// class Parser {

//   public:
//     Parser(const std::string source);
//     ~Parser();

//     std::vector<AstNode*> parse();

//   private:
//     Token* currentToken();

//     AstNode* Parse();

//     AstNode* parseExpr();

//     /// @brief 函数调用
//     AstNode* parseCall(char* keywords);

//     AstNode* parseApdu();

//     AstNode* parseTerm();

//     AstNode* parseFactor();

//     Token* advance();

//     Token* rollback();

//   private:
//     std::string source_;
//     int token_index_;
//     std::vector<Token> v_tokens_;
//     Token* current_token_;
// };

// } // namespace interpreter

// } // namespace zel