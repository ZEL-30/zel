#include "parser_result.h"

CParserResult::CParserResult() {
    error_ = NULL;
    node_ = NULL;
    advance_count_ = 0;
}

CParserResult* CParserResult::Success(CAstNode* node) {
    node_ = node;
    return this;
}

CParserResult* CParserResult::Failure(CError* error) {
    error_ = error;
    return this;
}

int CParserResult::RegisterAdvancement() { return advance_count_ += 1; }

CToken* CParserResult::Register(CToken* res) { return res; }

CAstNode* CParserResult::Register(CParserResult* res) {
    advance_count_ += res->advance_count_;
    if (res->error_ != NULL)
        error_ = res->error_;
    return res->node_;
}

CParserResult::~CParserResult() {}
