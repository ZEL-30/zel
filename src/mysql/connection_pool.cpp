/// @file connection_pool.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-02-07
/// @copyright Copyright (c) 2023 ZEL

#include "connection_pool.h"

#include "utility/logger.h"

using namespace zel::utility;

namespace zel {

namespace mysql {

ConnectionPool::ConnectionPool() : size_(0), ping_(3600), debug_(false) {}

ConnectionPool::~ConnectionPool() {
    for (auto it = pool_.begin(); it != pool_.end(); it++) {
        (*it)->close();
        delete (*it);
    }
    pool_.clear();
}

void ConnectionPool::create(const std::string& host,
                            int port,
                            const std::string& username,
                            const std::string& password,
                            const std::string& database,
                            const std::string& charset,
                            bool debug) {
    if (size_ <= 0) {
        log_error("connection pool size error");
        return;
    }
    debug_ = debug;
    for (int i = 0; i < size_; i++) {
        auto* conn = new Connection();
        conn->connect(host, port, username, password, database, charset, debug);
        if (ping_ > 0) {
            conn->set_ping(ping_);
        }
        put(conn);
    }
}

void ConnectionPool::size(int size) { size_ = size; }

void ConnectionPool::ping(int ping) { ping_ = ping; }

void ConnectionPool::put(Connection* conn) { pool_.push_back(conn); }

Connection* ConnectionPool::get() {
    if (pool_.empty()) {
        return nullptr;
    }
    auto conn = pool_.front();
    pool_.pop_front();
    if (!conn->ping()) {
        conn->reconnect();
    }
    return conn;
}

} // namespace mysql

} // namespace zel