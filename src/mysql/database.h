#pragma once

#include "connection.h"

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
    std::vector<std::map<std::string, zel::utility::Value>> query(const std::string& sql);
    std::vector<std::string> tables();
    bool exists(const std::string& table);
    std::vector<std::map<std::string, zel::utility::Value>> schema(const std::string& table);
    std::string primary_key(const std::string& table);
    std::string escape(const std::string& str);

    Connection* operator()();

  private:
    Connection* conn_;
};

} // namespace mysql
} // namespace zel
