#include "symbol_table.h"

#include <sstream>

namespace script {

namespace interpreter {

SymbolTable::SymbolTable() { m_symbol_table_.clear(); }

SymbolTable::~SymbolTable() {}

std::string SymbolTable::str() const {
    std::stringstream ss;

    for (auto iter = m_symbol_table_.begin(); iter != m_symbol_table_.end(); iter++) {
        ss << "  " << iter->first;
    }

    return ss.str();
}

void SymbolTable::symbolTable(const std::string& name, std::shared_ptr<Value> value) {
    m_symbol_table_[name] = value;
}

std::shared_ptr<Value> SymbolTable::symbolTable(const std::string& name) {
    auto iter = m_symbol_table_.find(name);
    std::shared_ptr<Value> value = nullptr;

    // func -> a -> 全局变量里有 a
    if (iter != m_symbol_table_.end()) {
        value = iter->second;
    }

    return value;
}

void SymbolTable::remove(std::string& name) { m_symbol_table_.erase(m_symbol_table_.find(name)); }

} // namespace interpreter

} // namespace zel