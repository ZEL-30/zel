// #include "parser.h"
// #include "interpreter/ast_node.h"
// #include <vector>



// namespace zel {

// namespace interpreter {

// Parser::Parser(const std::string source) {
//     source_ = source;

//     v_tokens_.clear();

// }

// Parser::~Parser() {}

// std::vector<AstNode*> Parser::parse() {
//     Lexer lexer(source_);
//     v_tokens_ = lexer.Tokenize();

//     std::vector<AstNode*> v_astnodes_;    

//     token_index_ = -1;
//     advance();
//     v_astnodes_.push_back(parseFactor());

//     // while (current_token_->type() != Token::END_OF_SOURCE) {
//     //     // throw std::logic_error("end of source error");
//     // }
//     return v_astnodes_;
// }

// AstNode* Parser::parseExpr() {

//     /*
//      * expr -> IDENTIFIER EQ expr
//      *      -> IDENTIFIER L
//      *      -> call ((STRING | LBRACKET) call)*
//      */

//     AstNode* res = new AstNode();

//     switch (current_token_->type_) {

//     case CToken::ScriptTokenType::IDENTIFIER: {
//         CToken* var_name = current_token_;
//         advance();

//         if (current_token_->type_ == CToken::ScriptTokenType::EQUAL) {
//             advance();

//             AstNode* expr = res->Register(Expr());

//             if (res->error_ != NULL)
//                 return res;

//             return res->Success(new CVarAssignNode(var_name, expr));
//         } else {
//             rollback();
//             return Apdu();
//         }
//     }

//     case CToken::ScriptTokenType::KEYWORDS: {
//         if (current_token_->Matches(CToken::ScriptTokenType::KEYWORDS, "crypto")) {
//             return Call("crypto");
//         } else if (current_token_->Matches(CToken::ScriptTokenType::KEYWORDS, "string")) {
//             return Call("string");
//         } else {
//         }
//     }

//     // apdu
//     default: {
//         return Apdu();
//     }
//     }
// }

// AstNode* Parser::parseCall(char* keywords) {

//     /*
//      * call -> (KEYWORDS:crypto | KEYWORDS:string) factor (LPAREN (expr (COMMA expr)*)? RPAREN)
//      *      -> expr (LPAREN (expr (COMMA expr)*)? RPAREN)
//      *      -> factor
//      */

//     AstNode* res = new AstNode();

//     advance();

//     AstNode* factor = res->Register(Factor());

//     if (current_token_->type_ == CToken::ScriptTokenType::LPAREN) {

//         advance();

//         std::vector<AstNode*> v_arg_nodes;

//         // add()
//         if (current_token_->type_ == CToken::ScriptTokenType::RPAREN) {
//             advance();
//         } else {
//             v_arg_nodes.push_back(res->Register(Expr()));
//             if (res->error_ != NULL) {
//                 return res;
//             }

//             while (current_token_->type_ == CToken::ScriptTokenType::COMMA) {
//                 advance();

//                 v_arg_nodes.push_back(res->Register(Expr()));
//                 if (res->error_ != NULL)
//                     return res;
//             }

//             if (current_token_->type_ != CToken::ScriptTokenType::RPAREN) {
//                 return res->Failure(new CInvalidSyntaxError(
//                     current_token_->pos_start_, current_token_->pos_end_, "Expected ',' or ')'"));
//             }

//             advance();
//         }

//         if (strcmp(keywords, "crypto") == 0)
//             return res->Success(new CCryptoClassNode(factor, v_arg_nodes));
//         else if (strcmp(keywords, "string") == 0)
//             return res->Success(new CStringClassNode(factor, v_arg_nodes));
//     }

//     return res->Success(factor);
// }

// AstNode* Parser::parseApdu() {
//     // apdu -> term (LPAREN (term (COMMA term)*)? RPAREN)

//     AstNode* res = new AstNode();

//     AstNode* term = res->Register(Term());

//     if (current_token_->type_ == CToken::ScriptTokenType::LPAREN) {

//         advance();

//         std::vector<AstNode*> v_arg_nodes;

//         // add()
//         if (current_token_->type_ == CToken::ScriptTokenType::RPAREN) {
//             advance();
//         } else {
//             AstNode* factor = res->Register(Factor());
//             if (res->error_ != NULL) {
//                 return res;
//             }
//             // ([divbuf]9000)
//             if (current_token_->type_ == CToken::ScriptTokenType::STRING) {
//                 AstNode* factor_end = res->Register(Factor());
//                 factor = new CApduResultNode(factor->var_name_token_, factor_end);
//             }

//             v_arg_nodes.push_back(factor);

//             while (current_token_->type_ == CToken::ScriptTokenType::COMMA) {
//                 advance();

//                 v_arg_nodes.push_back(res->Register(Factor()));
//                 if (res->error_ != NULL)
//                     return res;
//             }

//             if (current_token_->type_ != CToken::ScriptTokenType::RPAREN) {

//                 return res->Failure(new CInvalidSyntaxError(
//                     current_token_->pos_start_, current_token_->pos_end_, "Expected ',' or ')'"));
//             }

//             advance();
//         }
//         return res->Success(new CApduNode(term, v_arg_nodes));
//     }

//     return res->Success(term);
// }

// AstNode* Parser::parseTerm() {
//     // factor ((STRING | LBRACKET) factor)*
//     std::vector<CToken::ScriptTokenType> ops = {CToken::ScriptTokenType::STRING,
//                                                 CToken::ScriptTokenType::LBRACKET};
//     return BinOp(&Parser::Factor, ops);
// }


// AstNode* Parser::parseFactor() {
//     /*
//      * atom -> STRING | IDENTIFIER
//      *      -> LBRACKET IDENTIFIER RBRACKET
//      */

//     Token* token = current_token_;
//     switch (token->type()) {

//     // STRING | IDENTIFIER
//     case Token::Type::IDENTIFIER: {
//         advance();
//         return new VarAccessNode(token);
//         break;
//     }
//     case Token::Type::STRING: {
//         advance();
//         return new StringNode(*token);
//     }

//     // LBRACKET IDENTIFIER RBRACKET
//     case Token::Type::LBRACKET: {
//         // advance();

//         // AstNode* identifier = new CVarAccessNode(current_token_);
//         // advance();

//         // if (current_token_->type_ == Token::Type::RBRACKET) {
//         //     advance();
//         //     return res->Success(identifier);
//         // } else {
//         //     // 返回报错
//         //     return res->Failure(new CInvalidSyntaxError(
//         //         current_token_->pos_start_, current_token_->pos_end_, "Expected ']' "));
//         // }
//         break;
//     }

//     default:
//         advance();
//         break;
//     }
// }

// Token* Parser::currentToken() { return &v_tokens_[token_index_]; }

// Token* Parser::advance() {
//     token_index_ += 1;
//     if (token_index_ < v_tokens_.size()) {
//         current_token_ = &v_tokens_[token_index_];
//     }

//     return current_token_;
// }

// Token* Parser::rollback() {
//     token_index_ -= 1;
//     if (token_index_ < v_tokens_.size()) {
//         current_token_ = &v_tokens_[token_index_];
//     }

//     return current_token_;
// }

// } // namespace interpreter

// } // namespace zel