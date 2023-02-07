#pragma once

// 修复 windows 系统 min,max 宏冲突
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "batch.h"
#include "utility/logger.h"
#include "utility/value.h"
using namespace zel::utility;

#include <map>
#include <mysql.h>
#include <string.h>
#include <string>
#include <vector>


namespace zel {
namespace mysql {

class Connection {
  public:
    Connection();
    ~Connection();

    bool connect(const std::string& host,
                 int port,
                 const std::string& username,
                 const std::string& password,
                 const std::string& database,
                 const std::string& charset,
                 bool debug);
    bool reconnect();
    void close();
    bool ping();
    void set_ping(int seconds);
    std::string escape(const std::string& str);
    std::string quote(const std::string& str) const;
    std::vector<std::string> tables();
    std::vector<std::map<std::string, zel::utility::Value>> schema(const std::string& table);
    bool table_exists(const std::string& table);
    std::string primary_key(const std::string& table);

    int insert(const std::string& sql);
    bool execute(const std::string& sql);
    std::map<std::string, zel::utility::Value> fetchone(const std::string& sql);
    std::vector<std::map<std::string, zel::utility::Value>> fetchall(const std::string& sql);

    template <typename T>
    Batch<T> batch(const std::string& sql);

    // transaction
    bool auto_commit();
    void auto_commit(bool auto_commit);
    bool begin();
    bool rollback();
    bool commit();
    bool savepoint(const std::string& sp);
    bool rollback_savepoint(const std::string& sp);
    bool release_savepoint(const std::string& sp);

  private:
    MYSQL mysql_;
    std::string host_;
    int port_;
    std::string username_;
    std::string password_;
    std::string database_;
    std::string charset_;
    bool debug_;
    bool auto_commit_;
    int ping_;
    int last_ping_time_;
    char quote_;
};

template <typename T>
Batch<T> Connection::batch(const std::string& sql) {
    Batch<T> batch;
    if (debug_) {
        log_debug("sql: %s", sql.c_str());
    }
    int ret = mysql_real_query(&mysql_, sql.data(), sql.size());
    if (ret != 0) {
        log_error(
            "mysql_real_query errno:%d error:%s", mysql_errno(&mysql_), mysql_error(&mysql_));
        return batch;
    }
    MYSQL_RES* res = mysql_store_result(&mysql_);
    if (res == NULL) {
        log_error(
            "mysql_store_result errno:%d error:%s", mysql_errno(&mysql_), mysql_error(&mysql_));
        return batch;
    }
    int rows = mysql_num_rows(res);
    batch.total(rows);
    batch.result(res);
    return batch;
}

} // namespace mysql
} // namespace zel
