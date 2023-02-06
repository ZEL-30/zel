#pragma once

#include <mysql/connection.h>

namespace zel {
namespace mysql {

class Database {
  public:
    Database();
    Database(Connection* conn);
    ~Database();

    bool connect(const std::string& host,
                 int port,
                 const std::string& username,
                 const std::string& password,
                 const std::string& database,
                 const std::string& charset = "utf8",
                 bool debug = false);
    void close();
    void execute(const std::string& sql);
    std::vector<std::map<std::string, Value>> query(const std::string& sql);
    std::vector<std::string> tables();
    bool exists(const std::string& table);
    std::vector<std::map<std::string, Value>> schema(const std::string& table);
    std::string primary_key(const std::string& table);
    std::string escape(const std::string& str);

    Connection* operator()();

  private:
    Connection* m_conn;
};

} // namespace mysql
} // namespace zel
