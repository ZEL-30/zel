#pragma once

#include "mysql/connection.h"
// #include "mysql/database.h"
// #include "utility/logger.h"
#include "utility/value.h"

#include <iostream>
#include <map>

#include <vector>

namespace zel {

namespace mysql {

template <typename T>
class Model {
  public:
    Model();
    Model(Connection* conn);
    virtual ~Model();

    virtual std::string table() const = 0;
    virtual std::string primary_key() const;

    template <typename P, typename... Args>
    T& select(P head, Args... args);
    T& select() { return *dynamic_cast<T*>(this); }

    T& where(const std::string& cond);
    T& where(const std::string& field, const Value& value);
    T&
    where(const std::string& field, const std::string& op, const Value& value);
    T& where(const std::string& field,
             const std::string& op,
             const std::vector<Value>& values);
    T& where(const std::string& field,
             const std::string& op,
             const Value& min,
             const Value& max);

    T& alias(const std::string& alias);
    T& join(const std::string& table,
            const std::string& alias,
            const std::string& on,
            const std::string& type = "inner");
    T& group(const std::string& group);
    T& having(const std::string& having);
    T& order(const std::string& order);
    T& offset(int offset);
    T& limit(int limit);

    Value get(const std::string& field) const;
    void set(const std::string& field, const Value& value);

    Value operator()(const std::string& field);  // get
    Value& operator[](const std::string& field); // set

    std::string sql() const;
    std::string str() const;

    bool save();
    void remove();
    void update(const std::map<std::string, Value>& fields);
    void update(const T& row);
    void insert(const std::vector<T>& rows);
    void truncate();

    int count(const std::string& field = "*");
    double sum(const std::string& field);
    double min(const std::string& field);
    double max(const std::string& field);
    double average(const std::string& field);
    bool exists();
    Value scalar(const std::string& field);
    std::vector<Value> column(const std::string& field);

    T one();
    std::vector<T> all();

    Batch<T> batch(int size);

  protected:
    string build_select_sql() const;
    string build_join_sql() const;
    string build_where_sql() const;
    string build_other_sql() const;
    string build_simple_sql(const std::string& field,
                            const std::string& func = "") const;

  protected:
    Connection* m_conn;
    std::vector<std::string> m_select;
    std::map<std::string, Value> m_old_fields;
    std::map<std::string, Value> m_new_fields;
    std::map<std::string, Value> m_update;
    std::vector<std::string> m_where;
    std::vector<std::string> m_join;
    std::string m_alias;
    std::string m_order;
    std::string m_group;
    std::string m_having;
    int m_offset;
    int m_limit;
};

} // namespace mysql
} // namespace zel