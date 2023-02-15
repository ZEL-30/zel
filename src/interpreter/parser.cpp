#include "parser.h"

#include "ast_node.h"

#include <memory>
#include <string.h>
#include <vector>

namespace zel {

namespace interpreter {

Parser::Parser(const std::string& source) { source_ = source; }

Parser::~Parser() {}

std::vector<std::shared_ptr<AstNode>> Parser::parse() {

    Lexer lexer(source_);

    v_tokens_ = lexer.Tokenize();

    token_idx_ = -1;
    advance();
    std::vector<std::shared_ptr<AstNode>> v_ats_nodes_;

    while (current_token_->type() != Token::END_OF_SOURCE) {

        if (current_token_->type() != Token::END_OF_LINE &&
            current_token_->type() != Token::COMMENT) {
            auto res = parseExpr();

            v_ats_nodes_.push_back(res);
        }

        advance();
    }

    return v_ats_nodes_;
}

std::shared_ptr<AstNode> Parser::parseExpr() {

    /*
     * expr -> IDENTIFIER EQ expr
     *      -> IDENTIFIER L
     *      -> call ((STRING | LBRACKET) call)*
     */

    switch (current_token_->type()) {

    case Token::IDENTIFIER: {
        std::shared_ptr<Token> var_name = current_token_;
        advance();

        if (current_token_->type() == Token::EQUAL) {
            advance();

            std::shared_ptr<AstNode> expr = parseExpr();

            // return new VarAssignNode(var_name, expr);
            return std::make_shared<VarAssignNode>(var_name, expr);
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

std::shared_ptr<AstNode> Parser::parseCall(char* keywords) {

    /*
     * call -> (KEYWORDS:crypto | KEYWORDS:string) factor (LPAREN (expr (COMMA expr)*)? RPAREN)
     *      -> expr (LPAREN (expr (COMMA expr)*)? RPAREN)
     *      -> factor
     */

    advance();

    std::shared_ptr<AstNode> factor = parseFactor();

    if (current_token_->type() == Token::LPAREN) {

        advance();

        std::vector<std::shared_ptr<AstNode>> v_arg_nodes;

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
            return std::make_shared<CryptoClassNode>(factor, v_arg_nodes);
        else if (strcmp(keywords, "string") == 0)
            return std::make_shared<StringClassNode>(factor, v_arg_nodes);
    }

    return factor;
}

std::shared_ptr<AstNode> Parser::parseApdu() {
    // apdu -> term (LPAREN (term (COMMA term)*)? RPAREN)

    std::shared_ptr<AstNode> term = parseTerm();

    if (current_token_->type() == Token::LPAREN) {

        advance();

        std::vector<std::shared_ptr<AstNode>> v_arg_nodes;

        // add()
        if (current_token_->type() == Token::RPAREN) {
            advance();
        } else {
            std::shared_ptr<AstNode> factor = parseFactor();
            // ([divbuf]9000)
            if (current_token_->type() == Token::STRING) {
                std::shared_ptr<AstNode> factor_end = parseFactor();
                factor = std::make_shared<ApduResultNode>(factor->var_name_token_, factor_end);
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
        return std::make_shared<ApduNode>(term, v_arg_nodes);
    }
    return term;
}

std::shared_ptr<AstNode> Parser::parseTerm() {
    // factor ((STRING | LBRACKET) factor)*
    std::vector<Token::Type> ops = {Token::STRING, Token::LBRACKET};
    return binOp(&Parser::parseFactor, ops);
}

std::shared_ptr<AstNode> Parser::parseFactor() {
    /*
     * atom -> STRING | IDENTIFIER
     *      -> LBRACKET IDENTIFIER RBRACKET
     */

    std::shared_ptr<Token> token = current_token_;
    switch (token->type()) {

    // STRING | IDENTIFIER
    case Token::IDENTIFIER: {

        advance();
        return std::make_shared<VarAccessNode>(token);
    }
    case Token::STRING: {
        advance();
        return std::make_shared<StringNode>(token);
    }

    // LBRACKET IDENTIFIER RBRACKET
    case Token::LBRACKET: {
        advance();

        std::shared_ptr<AstNode> identifier = std::make_shared<VarAccessNode>(current_token_);
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

std::shared_ptr<AstNode> Parser::binOp(std::shared_ptr<AstNode> (Parser::*func_a)(),
                                       std::vector<Token::Type>& ops,
                                       std::shared_ptr<AstNode> (Parser::*func_b)()) {

    if (func_b == NULL)
        func_b = func_a;

    // 递归调用， 构建AST

    std::shared_ptr<AstNode> left = (this->*func_a)();

    while (current_token_->type() == ops[0] || current_token_->type() == ops[1]) {
        // Token *op_token = current_token_;
        // advance();
        std::shared_ptr<Token> op_token = std::make_shared<Token>("+", Token::PLUS);
        std::shared_ptr<AstNode> right = (this->*func_b)();

        left = std::make_shared<BinOpNode>(left, op_token, right);
    }

    return left;
}

std::shared_ptr<Token> Parser::advance() {
    token_idx_ += 1;
    if (token_idx_ < v_tokens_.size()) {
        current_token_ = v_tokens_[token_idx_];
    }

    return current_token_;
}

std::shared_ptr<Token> Parser::rollback() {
    token_idx_ -= 1;
    if (token_idx_ < v_tokens_.size()) {
        current_token_ = v_tokens_[token_idx_];
    }

    return current_token_;
}

} // namespace interpreter

} // namespace zel