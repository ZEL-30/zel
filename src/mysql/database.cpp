/// @file database.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-02-07
/// @copyright Copyright (c) 2023 ZEL

#include "database.h"

using namespace zel::utility;

namespace zel {

namespace mysql {

Database::Database() : conn_(nullptr) {}

Database::Database(Connection* conn) : conn_(conn) {}

Database::~Database() {}

bool Database::connect(const std::string& host,
                       int port,
                       const std::string& username,
                       const std::string& password,
                       const std::string& database,
                       const std::string& charset,
                       bool debug) {
    conn_ = new Connection();
    return conn_->connect(host, port, username, password, database, charset, debug);
}

void Database::close() {
    if (conn_ != nullptr) {
        conn_->close();
        delete conn_;
    }
}

void Database::execute(const std::string& sql) { conn_->execute(sql); }

std::vector<std::map<std::string, Value>> Database::query(const std::string& sql) {
    return conn_->fetchall(sql);
}

std::vector<std::string> Database::tables() { return conn_->tables(); }

bool Database::exists(const std::string& table) { return conn_->table_exists(table); }

std::vector<std::map<std::string, Value>> Database::schema(const std::string& table) {
    return conn_->schema(table);
}

std::string Database::primary_key(const std::string& table) { return conn_->primary_key(table); }

std::string Database::escape(const std::string& str) { return conn_->escape(str); }

Connection* Database::operator()() { return conn_; }

} // namespace mysql

} // namespace zel