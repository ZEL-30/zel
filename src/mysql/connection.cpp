/// @file connection.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief 
/// @version 0.1
/// @date 2023-02-07
/// @copyright Copyright (c) 2023 ZEL

#include "connection.h"

#include <ctime>
#include <iterator>
#include <sstream>


using namespace zel::utility;

namespace zel {

namespace mysql {

Connection::Connection() {
    mysql_init(&mysql_);
    ping_ = 3600;
    last_ping_time_ = time(nullptr);
    quote_ = '`';
}

Connection::~Connection() {}

bool Connection::connect(const std::string& host,
                         int port,
                         const std::string& username,
                         const std::string& password,
                         const std::string& database,
                         const std::string& charset,
                         bool debug) {
    host_ = host;
    port_ = port;
    username_ = username;
    database_ = database;
    charset_ = charset;
    debug_ = debug;
    mysql_options(&mysql_, MYSQL_SET_CHARSET_NAME, charset.c_str());
    MYSQL* res = mysql_real_connect(&mysql_,
                                    host.c_str(),
                                    username.c_str(),
                                    password.c_str(),
                                    database.c_str(),
                                    port,
                                    NULL,
                                    0);
    if (res == NULL) {
        log_error(
            "mysql_real_connect errno:%d error:%s", mysql_errno(&mysql_), mysql_error(&mysql_));
        return false;
    }
    if (debug) {
        log_debug("mysql connect success: host=%s port=%d username=%s database=%s",
                  host.c_str(),
                  port,
                  username.c_str(),
                  database.c_str());
    }
    auto_commit_ = true;
    mysql_autocommit(&mysql_, 1);
    return true;
}

bool Connection::reconnect() {
    close();
    return connect(host_, port_, username_, password_, database_, charset_, debug_);
}

void Connection::close() {
    if (debug_) {
        log_debug("mysql close: host=%s port=%d username=%s database=%s",
                  host_.c_str(),
                  port_,
                  username_.c_str(),
                  database_.c_str());
    }
    mysql_close(&mysql_);
}

bool Connection::ping() {
    int current_time = time(nullptr);
    if (current_time - last_ping_time_ > ping_) {
        last_ping_time_ = current_time;
        return mysql_ping(&mysql_) == 0;
    }
    return true;
}

void Connection::set_ping(int seconds) { ping_ = seconds; }

std::string Connection::escape(const std::string& str) {
    int size = 2 * str.size() + 1;
    char* buf = new char[size];
    memset(buf, 0, size);
    int len = mysql_real_escape_string(&mysql_, buf, str.c_str(), str.size());
    std::string ret = std::string(buf, len);
    delete[] buf;
    return std::move(ret);
}

std::string Connection::quote(const std::string& str) const {
    char separator = '.';
    size_t index = str.find(separator);
    if (index == std::string::npos) {
        std::ostringstream oss;
        oss << quote_ << str << quote_;
        return oss.str();
    }

    std::vector<std::string> output;
    std::stringstream ss(str);
    std::string item;
    while (getline(ss, item, separator)) {
        output.push_back(item);
    }
    std::ostringstream oss;
    for (auto it = output.begin(); it != output.end(); it++) {
        if (it != output.begin()) {
            oss << ".";
        }
        oss << quote_ << (*it) << quote_;
    }
    return oss.str();
}

std::vector<std::string> Connection::tables() {
    std::vector<std::string> all;
    MYSQL_RES* res = mysql_list_tables(&mysql_, NULL);
    if (res == NULL) {
        log_error(
            "mysql_list_tables errno:%d error:%s", mysql_errno(&mysql_), mysql_error(&mysql_));
        return all;
    }
    int fields = mysql_num_fields(res);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL) {
        for (int i = 0; i < fields; i++) {
            char* data = row[i];
            all.push_back(data);
        }
    }
    mysql_free_result(res);
    return all;
}

std::vector<std::map<std::string, Value>> Connection::schema(const std::string& table) {
    std::ostringstream oss;
    oss << "select column_name,column_key,data_type,extra,column_comment from "
           "information_schema.columns where table_schema='"
        << escape(database_) << "' and table_name='" << escape(table) << "'";
    const std::string& sql = oss.str();
    return fetchall(sql);
}

bool Connection::table_exists(const std::string& table) {
    std::ostringstream oss;
    oss << "select table_name from information_schema.tables WHERE "
           "table_schema='"
        << database_ << "' and table_name='" << table << "'";
    const std::string& sql = oss.str();
    auto one = fetchone(sql);
    return !one.empty();
}

std::string Connection::primary_key(const std::string& table) {
    auto all = schema(table);
    for (auto it = all.begin(); it != all.end(); it++) {
        if ((*it)["column_key"] == "PRI") {
            return (*it)["column_name"];
        }
    }
    return "";
}

int Connection::insert(const std::string& sql) {
    if (debug_) {
        log_debug("sql: %s", sql.c_str());
    }
    int ret = mysql_real_query(&mysql_, sql.data(), sql.size());
    if (ret != 0) {
        log_error(
            "mysql_real_query errno:%d error:%s", mysql_errno(&mysql_), mysql_error(&mysql_));
        return -1;
    }
    return mysql_insert_id(&mysql_);
}

bool Connection::execute(const std::string& sql) {
    if (debug_) {
        log_debug("sql: %s", sql.c_str());
    }
    int ret = mysql_real_query(&mysql_, sql.data(), sql.size());
    if (ret != 0) {
        log_error(
            "mysql_real_query errno:%d error:%s", mysql_errno(&mysql_), mysql_error(&mysql_));
        return false;
    }
    return true;
}

std::map<std::string, Value> Connection::fetchone(const std::string& sql) {
    std::map<std::string, Value> one;
    if (debug_) {
        log_debug("sql: %s", sql.c_str());
    }
    int ret = mysql_real_query(&mysql_, sql.data(), sql.size());
    if (ret != 0) {
        log_error(
            "mysql_real_query errno:%d error:%s", mysql_errno(&mysql_), mysql_error(&mysql_));
        return one;
    }
    MYSQL_RES* res = mysql_store_result(&mysql_);
    if (res == NULL) {
        log_error(
            "mysql_store_result errno:%d error:%s", mysql_errno(&mysql_), mysql_error(&mysql_));
        return one;
    }
    int fields = mysql_num_fields(res);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL) {
        unsigned long* lengths = mysql_fetch_lengths(res);
        for (int i = 0; i < fields; i++) {
            MYSQL_FIELD* field = mysql_fetch_field_direct(res, i);
            char* data = row[i];
            unsigned int length = lengths[i];
            Value v;
            v = std::string(data, length);
            if (IS_NUM(field->type)) {
                if (length == 0) {
                    v.type(Value::V_NULL);
                } else {
                    if (field->type == MYSQL_TYPE_DECIMAL || field->type == MYSQL_TYPE_FLOAT ||
                        field->type == MYSQL_TYPE_DOUBLE) {
                        v.type(Value::V_DOUBLE);
                    } else {
                        v.type(Value::V_INT);
                    }
                }
            } else if (field->type == MYSQL_TYPE_NULL) {
                v.type(Value::V_NULL);
            } else {
                v.type(Value::V_STRING);
            }
            one[field->name] = v;
        }
    }
    mysql_free_result(res);
    return one;
}

std::vector<std::map<std::string, Value>> Connection::fetchall(const std::string& sql) {
    std::vector<std::map<std::string, Value>> all;
    if (debug_) {
        log_debug("sql: %s", sql.c_str());
    }
    int ret = mysql_real_query(&mysql_, sql.data(), sql.size());
    if (ret != 0) {
        log_error(
            "mysql_real_query errno:%d error:%s", mysql_errno(&mysql_), mysql_error(&mysql_));
        return all;
    }
    MYSQL_RES* res = mysql_store_result(&mysql_);
    if (res == NULL) {
        log_error(
            "mysql_store_result errno:%d error:%s", mysql_errno(&mysql_), mysql_error(&mysql_));
        return all;
    }
    int fields = mysql_num_fields(res);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL) {
        std::map<std::string, Value> one;
        unsigned long* lengths = mysql_fetch_lengths(res);
        for (int i = 0; i < fields; i++) {
            MYSQL_FIELD* field = mysql_fetch_field_direct(res, i);
            char* data = row[i];
            unsigned int length = lengths[i];
            Value v;
            v = std::string(data, length);
            if (IS_NUM(field->type)) {
                if (length == 0) {
                    v.type(Value::V_NULL);
                } else {
                    if (field->type == MYSQL_TYPE_DECIMAL || field->type == MYSQL_TYPE_FLOAT ||
                        field->type == MYSQL_TYPE_DOUBLE) {
                        v.type(Value::V_DOUBLE);
                    } else {
                        v.type(Value::V_INT);
                    }
                }
            } else if (field->type == MYSQL_TYPE_NULL) {
                v.type(Value::V_NULL);
            } else {
                v.type(Value::V_STRING);
            }
            one[field->name] = v;
        }
        all.push_back(one);
    }
    mysql_free_result(res);
    return all;
}

bool Connection::auto_commit() { return auto_commit_; }

void Connection::auto_commit(bool auto_commit) {
    auto_commit_ = auto_commit;
    if (auto_commit) {
        mysql_autocommit(&mysql_, 1);
    } else {
        mysql_autocommit(&mysql_, 0);
    }
}

bool Connection::begin() {
    std::string sql = "begin";
    auto_commit(false);
    if (execute(sql)) {
        return true;
    }
    auto_commit(true);
    return false;
}

bool Connection::rollback() {
    std::string sql = "rollback";
    if (execute(sql)) {
        auto_commit(true);
        return true;
    }
    return false;
}

bool Connection::commit() {
    std::string sql = "commit";
    if (execute(sql)) {
        auto_commit(true);
        return true;
    }
    return false;
}

bool Connection::savepoint(const std::string& sp) {
    std::ostringstream oss;
    oss << "savepoint " << sp;
    const std::string& sql = oss.str();
    return execute(sql);
}

bool Connection::rollback_savepoint(const std::string& sp) {
    std::ostringstream oss;
    oss << "rollback to savepoint " << sp;
    const std::string& sql = oss.str();
    return execute(sql);
}

bool Connection::release_savepoint(const std::string& sp) {
    std::ostringstream oss;
    oss << "release savepoint " << sp;
    const std::string& sql = oss.str();
    return execute(sql);
}

} // namespace mysql

} // namespace zel