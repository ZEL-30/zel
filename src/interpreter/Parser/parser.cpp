#include "parser.h"

CParser::CParser(char* file_name, char* source) {
    strcpy_s(file_name_, file_name);
    strcpy_s(source_, source);
}

CToken* CParser::Advance() {
    token_idx_ += 1;
    if (token_idx_ < v_tokens_.size()) {
        current_token_ = v_tokens_[token_idx_];
    }

    return current_token_;
}

CToken* CParser::Rollback() {
    token_idx_ -= 1;
    if (token_idx_ < v_tokens_.size()) {
        current_token_ = v_tokens_[token_idx_];
    }

    return current_token_;
}

std::vector<CParserResult*> CParser::AstNodesList() {

    // CLexer Lexer(file_name_, source_);
    CLexer* Lexer = new CLexer(file_name_, source_);

    v_tokens_list_ = Lexer->MakeTokensList();

    for (int i = 0; i < v_tokens_list_.size(); i++) {

        v_tokens_ = v_tokens_list_[i];

        v_ats_nodes_.push_back(Parse());
    }

    delete Lexer;

    return v_ats_nodes_;
}

CParserResult* CParser::Parse() {

    token_idx_ = -1;
    Advance();

    CParserResult* res = Expr();

    if (res->error_ != nullptr && current_token_->type_ != CToken::ScriptTokenType::END_OF_SOURCE)
        // 返回报错
        return res->Failure(new CInvalidSyntaxError(
            current_token_->pos_start_, current_token_->pos_end_, "Expected 'STRING'"));

    return res;
}

CParserResult* CParser::Expr() {

    /*
     * expr -> IDENTIFIER EQ expr
     *      -> IDENTIFIER L
     *      -> call ((STRING | LBRACKET) call)*
     */

    CParserResult* res = new CParserResult();

    switch (current_token_->type_) {

    case CToken::ScriptTokenType::IDENTIFIER: {
        CToken* var_name = current_token_;
        res->RegisterAdvancement();
        Advance();

        if (current_token_->type_ == CToken::ScriptTokenType::EQUAL) {
            res->RegisterAdvancement();
            Advance();

            CAstNode* expr = res->Register(Expr());

            if (res->error_ != NULL)
                return res;

            return res->Success(new CVarAssignNode(var_name, expr));
        } else {
            Rollback();
            return Apdu();
        }
    }

    case CToken::ScriptTokenType::KEYWORDS: {
        if (current_token_->Matches(CToken::ScriptTokenType::KEYWORDS, "crypto")) {
            return Call("crypto");
        } else if (current_token_->Matches(CToken::ScriptTokenType::KEYWORDS, "string")) {
            return Call("string");
        } else {
        }
    }

    // apdu
    default: {
        return Apdu();
    }
    }
}

CParserResult* CParser::Call(char* keywords) {

    /*
     * call -> (KEYWORDS:crypto | KEYWORDS:string) factor (LPAREN (expr (COMMA expr)*)? RPAREN)
     *      -> expr (LPAREN (expr (COMMA expr)*)? RPAREN)
     *      -> factor
     */

    CParserResult* res = new CParserResult();

    res->RegisterAdvancement();
    Advance();

    CAstNode* factor = res->Register(Factor());

    if (current_token_->type_ == CToken::ScriptTokenType::LPAREN) {

        res->RegisterAdvancement();
        Advance();

        std::vector<CAstNode*> v_arg_nodes;

        // add()
        if (current_token_->type_ == CToken::ScriptTokenType::RPAREN) {
            res->RegisterAdvancement();
            Advance();
        } else {
            v_arg_nodes.push_back(res->Register(Expr()));
            if (res->error_ != NULL) {
                return res;
            }

            while (current_token_->type_ == CToken::ScriptTokenType::COMMA) {
                res->RegisterAdvancement();
                Advance();

                v_arg_nodes.push_back(res->Register(Expr()));
                if (res->error_ != NULL)
                    return res;
            }

            if (current_token_->type_ != CToken::ScriptTokenType::RPAREN) {
                return res->Failure(new CInvalidSyntaxError(
                    current_token_->pos_start_, current_token_->pos_end_, "Expected ',' or ')'"));
            }

            res->RegisterAdvancement();
            Advance();
        }

        if (strcmp(keywords, "crypto") == 0)
            return res->Success(new CCryptoClassNode(factor, v_arg_nodes));
        else if (strcmp(keywords, "string") == 0)
            return res->Success(new CStringClassNode(factor, v_arg_nodes));
    }

    return res->Success(factor);
}

CParserResult* CParser::Apdu() {
    // apdu -> term (LPAREN (term (COMMA term)*)? RPAREN)

    CParserResult* res = new CParserResult();

    CAstNode* term = res->Register(Term());

    if (current_token_->type_ == CToken::ScriptTokenType::LPAREN) {

        res->RegisterAdvancement();
        Advance();

        std::vector<CAstNode*> v_arg_nodes;

        // add()
        if (current_token_->type_ == CToken::ScriptTokenType::RPAREN) {
            res->RegisterAdvancement();
            Advance();
        } else {
            CAstNode* factor = res->Register(Factor());
            if (res->error_ != NULL) {
                return res;
            }
            // ([divbuf]9000)
            if (current_token_->type_ == CToken::ScriptTokenType::STRING) {
                CAstNode* factor_end = res->Register(Factor());
                factor = new CApduResultNode(factor->var_name_token_, factor_end);
            }

            v_arg_nodes.push_back(factor);

            while (current_token_->type_ == CToken::ScriptTokenType::COMMA) {
                res->RegisterAdvancement();
                Advance();

                v_arg_nodes.push_back(res->Register(Factor()));
                if (res->error_ != NULL)
                    return res;
            }

            if (current_token_->type_ != CToken::ScriptTokenType::RPAREN) {

                return res->Failure(new CInvalidSyntaxError(
                    current_token_->pos_start_, current_token_->pos_end_, "Expected ',' or ')'"));
            }

            res->RegisterAdvancement();
            Advance();
        }
        return res->Success(new CApduNode(term, v_arg_nodes));
    }

    return res->Success(term);
}

CParserResult* CParser::Term() {
    // factor ((STRING | LBRACKET) factor)*
    std::vector<CToken::ScriptTokenType> ops = {CToken::ScriptTokenType::STRING,
                                                CToken::ScriptTokenType::LBRACKET};
    return BinOp(&CParser::Factor, ops);
}

CParserResult* CParser::Factor() {
    /*
     * atom -> STRING | IDENTIFIER
     *      -> LBRACKET IDENTIFIER RBRACKET
     */

    CParserResult* res = new CParserResult();
    CToken* token = current_token_;
    switch (token->type_) {

    // STRING | IDENTIFIER
    case CToken::ScriptTokenType::IDENTIFIER: {

        res->RegisterAdvancement();
        Advance();
        return res->Success(new CVarAccessNode(token));
    }
    case CToken::ScriptTokenType::STRING: {
        res->RegisterAdvancement();
        Advance();
        return res->Success(new CStringNode(token));
    }

    // LBRACKET IDENTIFIER RBRACKET
    case CToken::ScriptTokenType::LBRACKET: {
        res->RegisterAdvancement();
        Advance();

        CAstNode* identifier = new CVarAccessNode(current_token_);
        res->RegisterAdvancement();
        Advance();

        if (current_token_->type_ == CToken::ScriptTokenType::RBRACKET) {
            res->RegisterAdvancement();
            Advance();
            return res->Success(identifier);
        } else {
            // 返回报错
            return res->Failure(new CInvalidSyntaxError(
                current_token_->pos_start_, current_token_->pos_end_, "Expected ']' "));
        }
    }

    default:
        return res->Failure(
            new CInvalidSyntaxError(current_token_->pos_start_,
                                    current_token_->pos_end_,
                                    "Expected int, float, '+', '-', '*', '/', '(' or ')'"));
    }
}

CParserResult* CParser::BinOp(CParserResult* (CParser::*func_a)(),
                              std::vector<CToken::ScriptTokenType>& ops,
                              CParserResult* (CParser::*func_b)()) {

    if (func_b == NULL)
        func_b = func_a;

    // 递归调用， 构建AST
    CParserResult* res = new CParserResult();

    CAstNode* left = res->Register((this->*func_a)());
    if (res->error_ != NULL)
        return res;

    while (current_token_->type_ == ops[0] || current_token_->type_ == ops[1]) {
        // CToken *op_token = current_token_;
        // res->RegisterAdvancement();
        // Advance();

        CToken* op_token = new CToken(CToken::ScriptTokenType::PLUS);
        CAstNode* right = res->Register((this->*func_b)());
        if (res->error_ != NULL)
            return res;

        left = new CBinOpNode(left, op_token, right);
    }

    return res->Success(left);
}

CParser::~CParser() {}