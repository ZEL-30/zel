#pragma once

#include "ast_node.h"
#include "lexer/lexer.h"
#include "parserresult/parser_result.h"
#include "zel.h"

#include <vector>

/// @brief 语法解析器
class CParser {

  public:
    CParser(char* file_name, char* source);

    std::vector<CParserResult*> AstNodesList();

    ~CParser();

  private:
    CParserResult* Parse();

    CParserResult* Expr();

    /// @brief 函数调用
    CParserResult* Call(char* keywords);

    CParserResult* Apdu();

    CParserResult* Term();

    CParserResult* Factor();

    CToken* Advance();

    CToken* Rollback();

    CParserResult* BinOp(CParserResult* (CParser::*func_a)(),
                         std::vector<CToken::ScriptTokenType>& ops,
                         CParserResult* (CParser::*func_b)() = NULL);

  private:
    int token_idx_;
    CToken* current_token_;
    char file_name_[1024]; // 文件名
    char source_[1024000]; // 原文

    std::vector<CToken*> v_tokens_;
    std::vector<CParserResult*> v_ats_nodes_;
    std::vector<std::vector<CToken*>> v_tokens_list_; // 词法单元容器列表
};
