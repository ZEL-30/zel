#pragma once

#include <list>
#include <mysql/connection.h>

namespace zel {
namespace mysql {

class ConnectionPool {
  public:
    ConnectionPool();
    ~ConnectionPool();

    void create(const string& host,
                int port,
                const string& username,
                const string& password,
                const string& database,
                const string& charset,
                bool debug = false);

    void size(int size);
    void ping(int ping);

    void put(Connection* conn);
    Connection* get();

  private:
    std::list<Connection*> m_pool;
    int m_size;
    int m_ping;
    bool m_debug;
};

} // namespace mysql
} // namespace zel