#include "parser.h"

#include "ast_node.h"

#include <vector>

namespace zel {

namespace interpreter {

Parser::Parser(const std::string source) { source_ = source; }

Token* Parser::advance() {
    token_idx_ += 1;
    if (token_idx_ < v_tokens_.size()) {
        current_token_ = &v_tokens_[token_idx_];
    }

    return current_token_;
}

Token* Parser::rollback() {
    token_idx_ -= 1;
    if (token_idx_ < v_tokens_.size()) {
        current_token_ = &v_tokens_[token_idx_];
    }

    return current_token_;
}

// std::vector<AstNode*> Parser::AstNodesList() {

//     // CLexer Lexer(file_name_, source_);
//     CLexer* Lexer = new CLexer(file_name_, source_);

//     v_tokens_list_ = Lexer->MakeTokensList();

//     for (int i = 0; i < v_tokens_list_.size(); i++) {

//         v_tokens_ = v_tokens_list_[i];

//         v_ats_nodes_.push_back(Parse());
//     }

//     delete Lexer;

//     return v_ats_nodes_;
// }

std::vector<AstNode*> Parser::parse() {

    Lexer lexer(source_);

    v_tokens_ = lexer.Tokenize();

    token_idx_ = -1;
    advance();

    std::vector<AstNode*> v_ats_nodes_;
    AstNode* res = NULL;
    while (current_token_->type() != Token::END_OF_SOURCE) {

        if (current_token_->type() == Token::END_OF_LINE) {
            advance();
            continue;
        }

        res = parseExpr();

        v_ats_nodes_.push_back(res);
    }

    return v_ats_nodes_;
}

AstNode* Parser::parseExpr() {

    /*
     * expr -> IDENTIFIER EQ expr
     *      -> IDENTIFIER L
     *      -> call ((STRING | LBRACKET) call)*
     */

    switch (current_token_->type()) {

    case Token::IDENTIFIER: {
        Token* var_name = current_token_;
        advance();

        if (current_token_->type() == Token::EQUAL) {
            advance();

            AstNode* expr = parseExpr();

            return new VarAssignNode(var_name, expr);
        } else {
            rollback();
            return parseApdu();
        }
    }

    case Token::KEYWORDS: {
        if (current_token_->matches(Token::KEYWORDS, "crypto")) {
            return parseCall("crypto");
        } else if (current_token_->matches(Token::KEYWORDS, "string")) {
            return parseCall("string");
        } else {
        }
    }

    // apdu
    default: {
        return parseApdu();
    }
    }
}

AstNode* Parser::parseCall(char* keywords) {

    /*
     * call -> (KEYWORDS:crypto | KEYWORDS:string) factor (LPAREN (expr (COMMA expr)*)? RPAREN)
     *      -> expr (LPAREN (expr (COMMA expr)*)? RPAREN)
     *      -> factor
     */

    advance();

    AstNode* factor = parseFactor();

    if (current_token_->type() == Token::LPAREN) {

        advance();

        std::vector<AstNode*> v_arg_nodes;

        // add()
        if (current_token_->type() == Token::RPAREN) {
            advance();
        } else {
            v_arg_nodes.push_back(parseExpr());
        }

        while (current_token_->type() == Token::COMMA) {
            advance();

            v_arg_nodes.push_back(parseExpr());
        }

        if (current_token_->type() != Token::RPAREN) {
        }

        advance();
        if (strcmp(keywords, "crypto") == 0)
            return new CryptoClassNode(factor, v_arg_nodes);
        else if (strcmp(keywords, "string") == 0)
            return new StringClassNode(factor, v_arg_nodes);
    }

    return factor;
}

AstNode* Parser::parseApdu() {
    // apdu -> term (LPAREN (term (COMMA term)*)? RPAREN)

    AstNode* term = parseTerm();

    if (current_token_->type() == Token::LPAREN) {

        advance();

        std::vector<AstNode*> v_arg_nodes;

        // add()
        if (current_token_->type() == Token::RPAREN) {
            advance();
        } else {
            AstNode* factor = parseFactor();
            // ([divbuf]9000)
            if (current_token_->type() == Token::STRING) {
                AstNode* factor_end = parseFactor();
                factor = new ApduResultNode(factor->var_name_token_, factor_end);
            }

            v_arg_nodes.push_back(factor);

            while (current_token_->type() == Token::COMMA) {
                advance();

                v_arg_nodes.push_back(parseFactor());
            }

            if (current_token_->type() != Token::RPAREN) {
            }

            advance();
        }
        return new ApduNode(term, v_arg_nodes);
    }
    return term;
}

AstNode* Parser::parseTerm() {
    // factor ((STRING | LBRACKET) factor)*
    std::vector<Token::Type> ops = {Token::STRING, Token::LBRACKET};
    return BinOp(&Parser::parseFactor, ops);
}

AstNode* Parser::parseFactor() {
    /*
     * atom -> STRING | IDENTIFIER
     *      -> LBRACKET IDENTIFIER RBRACKET
     */

    Token* token = current_token_;
    switch (token->type()) {

    // STRING | IDENTIFIER
    case Token::IDENTIFIER: {

        advance();
        return new VarAccessNode(token);
    }
    case Token::STRING: {
        advance();
        return new StringNode(token);
    }

    // LBRACKET IDENTIFIER RBRACKET
    case Token::LBRACKET: {
        advance();

        AstNode* identifier = new VarAccessNode(current_token_);
        advance();

        if (current_token_->type() == Token::RBRACKET) {
            advance();
            return identifier;
        } else {
            // 返回报错
        }
    }

    default:
        break;
    }
}

AstNode* Parser::BinOp(AstNode* (Parser::*func_a)(),
                       std::vector<Token::Type>& ops,
                       AstNode* (Parser::*func_b)()) {

    if (func_b == NULL)
        func_b = func_a;

    // 递归调用， 构建AST

    AstNode* left = (this->*func_a)();

    while (current_token_->type() == ops[0] || current_token_->type() == ops[1]) {
        // Token *op_token = current_token_;
        // advance();

        Token* op_token = new Token("+", Token::PLUS);
        AstNode* right = (this->*func_b)();

        left = new BinOpNode(left, op_token, right);
    }

    return left;
}

Parser::~Parser() {}

} // namespace interpreter

} // namespace zel