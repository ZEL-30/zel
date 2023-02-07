#pragma once

#include "database.h"

#include <stack>
#include <string>


namespace zel {
namespace mysql {

class Transaction {
  public:
    Transaction(Database& db);
    Transaction(Connection* conn);
    ~Transaction();

    void begin();
    void rollback();
    void commit();

  private:
    Connection* conn_;
    bool is_start_;
    std::stack<std::string> savepoints_;
    int counter_;
};

} // namespace mysql
} // namespace zel