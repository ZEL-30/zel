#pragma once

#include "mysql/database.h"

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
    Connection* m_conn;
    bool m_is_start;
    std::stack<string> m_savepoints;
    int m_counter;
};

} // namespace mysql
} // namespace zel