#pragma once

#include "value.h"

#include <map>
#include <string>

namespace script {

namespace interpreter {

class SymbolTable {

  public:
    SymbolTable();
    ~SymbolTable();

    std::string str() const;

    /// @brief 变量的赋值
    /// @param name 变量名
    /// @param value 变量的值
    void symbolTable(const std::string& name, std::shared_ptr<Value> value);

    /// @brief 获取符号(变量)的值
    /// @param name 变量名
    /// @return 变量的值
    std::shared_ptr<Value> symbolTable(const std::string& name);

    /// @brief 删除变量
    /// @param name 变量名
    void remove(std::string& name);

  private:
    std::map<std::string, std::shared_ptr<Value>> m_symbol_table_; // 符号表
};
} // namespace interpreter
} // namespace zel