#pragma once

#include "connection.h"
#include "utility/value.h"
using namespace zel::utility;

#include <iostream>
#include <map>
#include <sstream>
#include <string>
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
    T& where(const std::string& field, const std::string& op, const Value& value);
    T& where(const std::string& field, const std::string& op, const std::vector<Value>& values);
    T& where(const std::string& field, const std::string& op, const Value& min, const Value& max);

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
    std::string build_select_sql() const;
    std::string build_join_sql() const;
    std::string build_where_sql() const;
    std::string build_other_sql() const;
    std::string build_simple_sql(const std::string& field, const std::string& func = "") const;

  protected:
    Connection* conn_;
    std::vector<std::string> select_;
    std::map<std::string, Value> old_fields_;
    std::map<std::string, Value> new_fields_;
    std::map<std::string, Value> update_;
    std::vector<std::string> where_;
    std::vector<std::string> join_;
    std::string alias_;
    std::string order_;
    std::string group_;
    std::string having_;
    int offset_;
    int limit_;
};

template <typename T>
Model<T>::Model() : conn_(nullptr) {
    offset_ = 0;
    limit_ = 0;
}

template <typename T>
Model<T>::Model(Connection* conn) : conn_(conn) {
    offset_ = 0;
    limit_ = 0;
}

template <typename T>
Model<T>::~Model() {}

template <typename T>
std::string Model<T>::primary_key() const {
    return "id";
}

template <typename T>
template <typename P, typename... Args>
T& Model<T>::select(P head, Args... args) {
    select_.push_back(head);
    return select(args...);
}

template <typename T>
T& Model<T>::where(const std::string& cond) {
    where_.push_back(cond);
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::where(const std::string& field, const Value& value) {
    return where(field, "=", value);
}

template <typename T>
T& Model<T>::where(const std::string& field, const std::string& op, const Value& value) {
    std::ostringstream oss;
    oss << conn_->quote(field) << " " << op << " ";
    if (value.isString()) {
        oss << "'" << conn_->escape(value) << "'";
    } else {
        oss << (std::string)value;
    }
    where_.push_back(oss.str());
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::where(const std::string& field,
                   const std::string& op,
                   const std::vector<Value>& values) {
    std::ostringstream oss;
    oss << conn_->quote(field) << " " << op << " ";
    oss << "(";
    for (auto it = values.begin(); it != values.end(); it++) {
        if (it != values.begin()) {
            oss << ",";
        }
        if (it->isString()) {
            oss << "'" << conn_->escape(*it) << "'";
        } else {
            oss << (std::string)(*it);
        }
    }
    oss << ")";
    where_.push_back(oss.str());
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::where(const std::string& field,
                   const std::string& op,
                   const Value& min,
                   const Value& max) {
    std::ostringstream oss;
    oss << conn_->quote(field) << " " << op << " ";
    if (min.isString()) {
        oss << "'" << conn_->escape(min) << "'";
    } else {
        oss << (std::string)min;
    }
    oss << " and ";
    if (max.isString()) {
        oss << "'" << conn_->escape(max) << "'";
    } else {
        oss << (std::string)max;
    }
    where_.push_back(oss.str());
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::alias(const std::string& alias) {
    alias_ = alias;
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::join(const std::string& table,
                  const std::string& alias,
                  const std::string& on,
                  const std::string& type) {
    std::ostringstream oss;
    oss << type << " join " << conn_->quote(table) << " as " << conn_->quote(alias) << " on "
        << on;
    join_.push_back(oss.str());
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::group(const std::string& group) {
    group_ = group;
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::having(const std::string& having) {
    having_ = having;
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::order(const std::string& order) {
    order_ = order;
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::offset(int offset) {
    offset_ = offset;
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::limit(int limit) {
    limit_ = limit;
    return *dynamic_cast<T*>(this);
}

template <typename T>
Value Model<T>::get(const std::string& field) const {
    auto it = new_fields_.find(field);
    if (it != new_fields_.end()) {
        return it->second;
    }
    it = old_fields_.find(field);
    if (it != old_fields_.end()) {
        return it->second;
    }
    return Value();
}

template <typename T>
void Model<T>::set(const std::string& field, const Value& value) {
    new_fields_[field] = value;
}

template <typename T>
Value Model<T>::operator()(const std::string& field) {
    return get(field);
}

template <typename T>
Value& Model<T>::operator[](const std::string& field) {
    return new_fields_[field];
}

template <typename T>
std::string Model<T>::sql() const {
    std::ostringstream oss;
    oss << build_select_sql();
    if (alias_ != "") {
        oss << " as " << conn_->quote(alias_);
    }
    oss << build_join_sql();
    oss << build_where_sql();
    oss << build_other_sql();
    return oss.str();
}

template <typename T>
std::string Model<T>::str() const {
    std::map<std::string, Value> fields;
    for (auto it = old_fields_.begin(); it != old_fields_.end(); it++) {
        fields[it->first] = it->second;
    }
    for (auto it = new_fields_.begin(); it != new_fields_.end(); it++) {
        fields[it->first] = it->second;
    }
    std::ostringstream oss;
    oss << "{";
    for (auto it = fields.begin(); it != fields.end(); it++) {
        if (it == fields.begin()) {
            oss << "\"" << it->first << "\": ";
        } else {
            oss << ", \"" << it->first << "\": ";
        }
        switch (it->second.type()) {
        case Value::V_NULL:
            oss << "null";
            break;
        case Value::V_BOOL:
            oss << std::string(it->second);
            break;
        case Value::V_INT:
            oss << std::string(it->second);
            break;
        case Value::V_DOUBLE:
            oss << std::string(it->second);
            break;
        case Value::V_STRING:
            oss << "\"" << std::string(it->second) << "\"";
            break;
        default:
            break;
        }
    }
    oss << "}";
    return oss.str();
}

template <typename T>
std::string Model<T>::build_select_sql() const {
    std::ostringstream oss;
    oss << "select ";
    if (select_.empty()) {
        oss << "*";
    } else {
        for (auto it = select_.begin(); it != select_.end(); it++) {
            if (it == select_.begin()) {
                oss << (*it);
            } else {
                oss << "," << (*it);
            }
        }
    }
    oss << " from " << conn_->quote(table());
    return oss.str();
}

template <typename T>
std::string Model<T>::build_join_sql() const {
    if (join_.empty()) {
        return "";
    }
    std::ostringstream oss;
    oss << " ";
    for (auto it = join_.begin(); it != join_.end(); it++) {
        if (it == join_.begin()) {
            oss << *it;
        } else {
            oss << " " << *it;
        }
    }
    return oss.str();
}

template <typename T>
std::string Model<T>::build_where_sql() const {
    if (where_.empty()) {
        return "";
    }
    std::ostringstream oss;
    oss << " where ";
    for (auto it = where_.begin(); it != where_.end(); it++) {
        if (it == where_.begin()) {
            oss << (*it);
        } else {
            oss << " and " << (*it);
        }
    }
    return oss.str();
}

template <typename T>
std::string Model<T>::build_other_sql() const {
    std::ostringstream oss;
    if (!group_.empty()) {
        oss << " group by " << group_;
    }
    if (!having_.empty()) {
        oss << " having " << having_;
    }
    if (!order_.empty()) {
        oss << " order by " << order_;
    }
    if (limit_ > 0) {
        if (offset_ > 0) {
            oss << " limit " << limit_ << " offset " << offset_;
        } else {
            oss << " limit " << limit_;
        }
    }
    return oss.str();
}

template <typename T>
std::string Model<T>::build_simple_sql(const std::string& field, const std::string& func) const {
    std::ostringstream oss;
    oss << "select ";
    if (func.empty()) {
        oss << conn_->quote(field);
    } else {
        oss << func << "(" << conn_->quote(field) << ")";
    }
    oss << " from " << conn_->quote(table());
    if (!alias_.empty()) {
        oss << " as " << conn_->quote(alias_);
    }
    oss << build_join_sql();
    oss << build_where_sql();
    oss << build_other_sql();
    return oss.str();
}

template <typename T>
bool Model<T>::save() {
    std::string pk = primary_key();
    if (old_fields_.find(pk) == old_fields_.end()) {
        // insert
        if (new_fields_.empty()) {
            return false;
        }
        std::ostringstream fields;
        std::ostringstream values;
        for (auto it = new_fields_.begin(); it != new_fields_.end(); it++) {
            if (it != new_fields_.begin()) {
                fields << ",";
                values << ",";
            }
            fields << conn_->quote(it->first);
            if (it->second.isString()) {
                values << "'" << conn_->escape(it->second) << "'";
            } else {
                values << (std::string)(it->second);
            }
        }
        std::ostringstream oss;
        oss << "insert into " << conn_->quote(table()) << "(" << fields.str() << ") values("
            << values.str() << ")";
        std::string sql = oss.str();
        int last_id = conn_->insert(sql);
        new_fields_[pk] = last_id;
        return true;
    } else {
        // update
        if (new_fields_.empty()) {
            return false;
        }
        std::ostringstream update;
        for (auto it = new_fields_.begin(); it != new_fields_.end(); it++) {
            if (it != new_fields_.begin()) {
                update << ", ";
            }
            update << conn_->quote(it->first) << " = ";
            if (it->second.isString()) {
                update << "'" << conn_->escape(it->second) << "'";
            } else {
                update << (std::string)(it->second);
            }
        }
        std::ostringstream oss;
        oss << "update " << conn_->quote(table()) << " set " << update.str() << " where "
            << conn_->quote(pk) << " = " << (std::string)old_fields_[pk];
        std::string sql = oss.str();
        conn_->execute(sql);
        return true;
    }
}

template <typename T>
void Model<T>::remove() {
    std::ostringstream oss;
    oss << "delete from " << conn_->quote(table());
    std::string pk = primary_key();
    if (old_fields_.find(pk) == old_fields_.end()) {
        // oss << build_join_sql();
        oss << build_where_sql();
        oss << build_other_sql();
    } else {
        oss << " where " << conn_->quote(pk) << " = " << (std::string)old_fields_[pk];
    }
    std::string sql = oss.str();
    conn_->execute(sql);
}

template <typename T>
void Model<T>::update(const T& row) {
    update(row.new_fields_);
}

template <typename T>
void Model<T>::update(const std::map<std::string, Value>& fields) {
    if (fields.empty()) {
        return;
    }
    std::ostringstream oss;
    oss << "update " << conn_->quote(table()) << " set ";
    for (auto it = fields.begin(); it != fields.end(); it++) {
        if (it != fields.begin()) {
            oss << ", ";
        }
        oss << conn_->quote(it->first) << " = ";
        if (it->second.isString()) {
            oss << "'" << conn_->escape(it->second) << "'";
        } else {
            oss << (std::string)(it->second);
        }
    }
    // oss << build_join_sql();
    oss << build_where_sql();
    oss << build_other_sql();
    std::string sql = oss.str();
    conn_->execute(sql);
}

template <typename T>
void Model<T>::insert(const std::vector<T>& rows) {
    if (rows.empty()) {
        return;
    }
    std::ostringstream oss;
    oss << "insert into " << conn_->quote(table());
    oss << "(";
    for (auto it = rows[0].new_fields_.begin(); it != rows[0].new_fields_.end(); it++) {
        if (it != rows[0].new_fields_.begin()) {
            oss << ",";
        }
        oss << conn_->quote(it->first);
    }
    oss << ") values ";
    for (auto row = rows.begin(); row != rows.end(); row++) {
        if (row != rows.begin()) {
            oss << ",";
        }
        oss << "(";
        for (auto it = row->new_fields_.begin(); it != row->new_fields_.end(); it++) {
            if (it != row->new_fields_.begin()) {
                oss << ",";
            }
            if (it->second.isString()) {
                oss << "'" << conn_->escape(it->second) << "'";
            } else {
                oss << std::string(it->second);
            }
        }
        oss << ")";
    }
    std::string sql = oss.str();
    conn_->insert(sql);
}

template <typename T>
void Model<T>::truncate() {
    std::ostringstream oss;
    oss << "truncate table " << conn_->quote(table());
    std::string sql = oss.str();
    conn_->execute(sql);
}

template <typename T>
int Model<T>::count(const std::string& field) {
    std::ostringstream oss;
    oss << "select count(" << field << ") from " << conn_->quote(table());
    if (!alias_.empty()) {
        oss << " as " << conn_->quote(alias_);
    }
    oss << build_join_sql();
    oss << build_where_sql();
    oss << build_other_sql();
    std::string sql = oss.str();
    auto one = conn_->fetchone(sql);
    for (auto it = one.begin(); it != one.end(); it++) {
        return it->second;
    }
    return 0;
}

template <typename T>
double Model<T>::sum(const std::string& field) {
    std::string sql = build_simple_sql(field, "sum");
    auto one = conn_->fetchone(sql);
    for (auto it = one.begin(); it != one.end(); it++) {
        return it->second;
    }
    return 0;
}

template <typename T>
double Model<T>::min(const std::string& field) {
    std::string sql = build_simple_sql(field, "min");
    auto one = conn_->fetchone(sql);
    for (auto it = one.begin(); it != one.end(); it++) {
        return it->second;
    }
    return 0;
}

template <typename T>
double Model<T>::max(const std::string& field) {
    std::string sql = build_simple_sql(field, "max");
    auto one = conn_->fetchone(sql);
    for (auto it = one.begin(); it != one.end(); it++) {
        return it->second;
    }
    return 0;
}

template <typename T>
double Model<T>::average(const std::string& field) {
    std::string sql = build_simple_sql(field, "avg");
    auto one = conn_->fetchone(sql);
    for (auto it = one.begin(); it != one.end(); it++) {
        return it->second;
    }
    return 0;
}

template <typename T>
bool Model<T>::exists() {
    std::ostringstream oss;
    oss << "select exists(" << sql() << ")";
    std::string sql = oss.str();

    int total = 0;
    auto one = conn_->fetchone(sql);
    for (auto it = one.begin(); it != one.end(); it++) {
        total = it->second;
    }
    return total == 1;
}

template <typename T>
Value Model<T>::scalar(const std::string& field) {
    limit_ = 1;
    std::string sql = build_simple_sql(field);
    auto one = conn_->fetchone(sql);
    return one[field];
}

template <typename T>
std::vector<Value> Model<T>::column(const std::string& field) {
    std::string sql = build_simple_sql(field);
    auto all = conn_->fetchall(sql);
    std::vector<Value> result;
    for (auto it = all.begin(); it != all.end(); it++) {
        result.push_back((*it)[field]);
    }
    return result;
}

template <typename T>
T Model<T>::one() {
    limit_ = 1;
    T one(conn_);
    one.old_fields_ = conn_->fetchone(sql());
    return one;
}

template <typename T>
std::vector<T> Model<T>::all() {
    std::vector<T> all;
    std::vector<std::map<std::string, Value>> result = conn_->fetchall(sql());
    for (auto it = result.begin(); it != result.end(); it++) {
        T one(conn_);
        one.old_fields_ = (*it);
        all.push_back(one);
    }
    return all;
}

template <typename T>
Batch<T> Model<T>::batch(int size) {
    Batch<T> batch = conn_->batch<T>(sql());
    batch.size(size);
    return batch;
}

} // namespace mysql
} // namespace zel