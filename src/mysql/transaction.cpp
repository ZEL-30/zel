/// @file transaction.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-02-07
/// @copyright Copyright (c) 2023 ZEL

#include "transaction.h"

#include <sstream>

namespace zel {

namespace mysql {

Transaction::Transaction(Database& db) : conn_(db()) {
    is_start_ = false;
    counter_ = 0;
}

Transaction::Transaction(Connection* conn) : conn_(conn) {
    is_start_ = false;
    counter_ = 0;
}

Transaction::~Transaction() {}

void Transaction::begin() {
    if (is_start_) {
        std::ostringstream oss;
        oss << "sp" << counter_;
        const std::string& sp = conn_->quote(oss.str());
        savepoints_.push(sp);
        if (conn_->savepoint(sp)) {
            counter_ += 1;
        }
    } else {
        if (conn_->begin()) {
            is_start_ = true;
        }
    }
}

void Transaction::rollback() {
    if (!savepoints_.empty()) {
        const std::string& sp = savepoints_.top();
        if (conn_->rollback_savepoint(sp)) {
            savepoints_.pop();
        }
    } else {
        if (conn_->rollback()) {
            is_start_ = false;
        }
    }
}

void Transaction::commit() {
    if (!savepoints_.empty()) {
        const std::string& sp = savepoints_.top();
        if (conn_->release_savepoint(sp)) {
            savepoints_.pop();
        }
    } else {
        if (conn_->commit()) {
            is_start_ = false;
        }
    }
}

} // namespace mysql

} // namespace zel