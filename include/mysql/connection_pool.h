#pragma once

#include "connection.h"

#include <list>


namespace zel {
namespace mysql {

class ConnectionPool {
  public:
    ConnectionPool();
    ~ConnectionPool();

    void create(const std::string& host,
                int port,
                const std::string& username,
                const std::string& password,
                const std::string& database,
                const std::string& charset,
                bool debug = false);

    void size(int size);
    void ping(int ping);

    void put(Connection* conn);
    Connection* get();

  private:
    std::list<Connection*> pool_;
    int size_;
    int ping_;
    bool debug_;
};

} // namespace mysql
} // namespace zel