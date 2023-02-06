#include "mysql/model.h"

#include <sstream>
#include <string>

namespace zel {

namespace mysql {

template <typename T>
Model<T>::Model() : m_conn(nullptr) {
    m_offset = 0;
    m_limit = 0;
}

template <typename T>
Model<T>::Model(Connection* conn) : m_conn(conn) {
    m_offset = 0;
    m_limit = 0;
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
    m_select.push_back(head);
    return select(args...);
}

template <typename T>
T& Model<T>::where(const std::string& cond) {
    m_where.push_back(cond);
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::where(const std::string& field, const Value& value) {
    return where(field, "=", value);
}

template <typename T>
T& Model<T>::where(const std::string& field,
                   const std::string& op,
                   const Value& value) {
    std::ostringstream oss;
    oss << m_conn->quote(field) << " " << op << " ";
    if (value.IsString()) {
        oss << "'" << m_conn->escape(value) << "'";
    } else {
        oss << (std::string)value;
    }
    m_where.push_back(oss.str());
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::where(const std::string& field,
                   const std::string& op,
                   const std::vector<Value>& values) {
    std::ostringstream oss;
    oss << m_conn->quote(field) << " " << op << " ";
    oss << "(";
    for (auto it = values.begin(); it != values.end(); it++) {
        if (it != values.begin()) {
            oss << ",";
        }
        if (it->IsString()) {
            oss << "'" << m_conn->escape(*it) << "'";
        } else {
            oss << (std::string)(*it);
        }
    }
    oss << ")";
    m_where.push_back(oss.str());
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::where(const std::string& field,
                   const std::string& op,
                   const Value& min,
                   const Value& max) {
    std::ostringstream oss;
    oss << m_conn->quote(field) << " " << op << " ";
    if (min.IsString()) {
        oss << "'" << m_conn->escape(min) << "'";
    } else {
        oss << (std::string)min;
    }
    oss << " and ";
    if (max.IsString()) {
        oss << "'" << m_conn->escape(max) << "'";
    } else {
        oss << (std::string)max;
    }
    m_where.push_back(oss.str());
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::alias(const std::string& alias) {
    m_alias = alias;
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::join(const std::string& table,
                  const std::string& alias,
                  const std::string& on,
                  const std::string& type) {
    std::ostringstream oss;
    oss << type << " join " << m_conn->quote(table) << " as "
        << m_conn->quote(alias) << " on " << on;
    m_join.push_back(oss.str());
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::group(const std::string& group) {
    m_group = group;
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::having(const std::string& having) {
    m_having = having;
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::order(const std::string& order) {
    m_order = order;
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::offset(int offset) {
    m_offset = offset;
    return *dynamic_cast<T*>(this);
}

template <typename T>
T& Model<T>::limit(int limit) {
    m_limit = limit;
    return *dynamic_cast<T*>(this);
}

template <typename T>
Value Model<T>::get(const std::string& field) const {
    auto it = m_new_fields.find(field);
    if (it != m_new_fields.end()) {
        return it->second;
    }
    it = m_old_fields.find(field);
    if (it != m_old_fields.end()) {
        return it->second;
    }
    return Value();
}

template <typename T>
void Model<T>::set(const std::string& field, const Value& value) {
    m_new_fields[field] = value;
}

template <typename T>
Value Model<T>::operator()(const std::string& field) {
    return get(field);
}

template <typename T>
Value& Model<T>::operator[](const std::string& field) {
    return m_new_fields[field];
}

template <typename T>
std::string Model<T>::sql() const {
    std::ostringstream oss;
    oss << build_select_sql();
    if (m_alias != "") {
        oss << " as " << m_conn->quote(m_alias);
    }
    oss << build_join_sql();
    oss << build_where_sql();
    oss << build_other_sql();
    return oss.str();
}

template <typename T>
std::string Model<T>::str() const {
    std::map<std::string, Value> fields;
    for (auto it = m_old_fields.begin(); it != m_old_fields.end(); it++) {
        fields[it->first] = it->second;
    }
    for (auto it = m_new_fields.begin(); it != m_new_fields.end(); it++) {
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
    if (m_select.empty()) {
        oss << "*";
    } else {
        for (auto it = m_select.begin(); it != m_select.end(); it++) {
            if (it == m_select.begin()) {
                oss << (*it);
            } else {
                oss << "," << (*it);
            }
        }
    }
    oss << " from " << m_conn->quote(table());
    return oss.str();
}

template <typename T>
std::string Model<T>::build_join_sql() const {
    if (m_join.empty()) {
        return "";
    }
    std::ostringstream oss;
    oss << " ";
    for (auto it = m_join.begin(); it != m_join.end(); it++) {
        if (it == m_join.begin()) {
            oss << *it;
        } else {
            oss << " " << *it;
        }
    }
    return oss.str();
}

template <typename T>
std::string Model<T>::build_where_sql() const {
    if (m_where.empty()) {
        return "";
    }
    std::ostringstream oss;
    oss << " where ";
    for (auto it = m_where.begin(); it != m_where.end(); it++) {
        if (it == m_where.begin()) {
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
    if (!m_group.empty()) {
        oss << " group by " << m_group;
    }
    if (!m_having.empty()) {
        oss << " having " << m_having;
    }
    if (!m_order.empty()) {
        oss << " order by " << m_order;
    }
    if (m_limit > 0) {
        if (m_offset > 0) {
            oss << " limit " << m_limit << " offset " << m_offset;
        } else {
            oss << " limit " << m_limit;
        }
    }
    return oss.str();
}

template <typename T>
std::string Model<T>::build_simple_sql(const std::string& field,
                                       const std::string& func) const {
    std::ostringstream oss;
    oss << "select ";
    if (func.empty()) {
        oss << m_conn->quote(field);
    } else {
        oss << func << "(" << m_conn->quote(field) << ")";
    }
    oss << " from " << m_conn->quote(table());
    if (!m_alias.empty()) {
        oss << " as " << m_conn->quote(m_alias);
    }
    oss << build_join_sql();
    oss << build_where_sql();
    oss << build_other_sql();
    return oss.str();
}

template <typename T>
bool Model<T>::save() {
    std::string pk = primary_key();
    if (m_old_fields.find(pk) == m_old_fields.end()) {
        // insert
        if (m_new_fields.empty()) {
            return false;
        }
        std::ostringstream fields;
        std::ostringstream values;
        for (auto it = m_new_fields.begin(); it != m_new_fields.end(); it++) {
            if (it != m_new_fields.begin()) {
                fields << ",";
                values << ",";
            }
            fields << m_conn->quote(it->first);
            if (it->second.IsString()) {
                values << "'" << m_conn->escape(it->second) << "'";
            } else {
                values << (std::string)(it->second);
            }
        }
        std::ostringstream oss;
        oss << "insert into " << m_conn->quote(table()) << "(" << fields.str()
            << ") values(" << values.str() << ")";
        std::string sql = oss.str();
        int last_id = m_conn->insert(sql);
        m_new_fields[pk] = last_id;
        return true;
    } else {
        // update
        if (m_new_fields.empty()) {
            return false;
        }
        std::ostringstream update;
        for (auto it = m_new_fields.begin(); it != m_new_fields.end(); it++) {
            if (it != m_new_fields.begin()) {
                update << ", ";
            }
            update << m_conn->quote(it->first) << " = ";
            if (it->second.IsString()) {
                update << "'" << m_conn->escape(it->second) << "'";
            } else {
                update << (std::string)(it->second);
            }
        }
        std::ostringstream oss;
        oss << "update " << m_conn->quote(table()) << " set " << update.str()
            << " where " << m_conn->quote(pk) << " = "
            << (std::string)m_old_fields[pk];
        std::string sql = oss.str();
        m_conn->execute(sql);
        return true;
    }
}

template <typename T>
void Model<T>::remove() {
    std::ostringstream oss;
    oss << "delete from " << m_conn->quote(table());
    std::string pk = primary_key();
    if (m_old_fields.find(pk) == m_old_fields.end()) {
        // oss << build_join_sql();
        oss << build_where_sql();
        oss << build_other_sql();
    } else {
        oss << " where " << m_conn->quote(pk) << " = "
            << (std::string)m_old_fields[pk];
    }
    std::string sql = oss.str();
    m_conn->execute(sql);
}

template <typename T>
void Model<T>::update(const T& row) {
    update(row.m_new_fields);
}

template <typename T>
void Model<T>::update(const std::map<std::string, Value>& fields) {
    if (fields.empty()) {
        return;
    }
    std::ostringstream oss;
    oss << "update " << m_conn->quote(table()) << " set ";
    for (auto it = fields.begin(); it != fields.end(); it++) {
        if (it != fields.begin()) {
            oss << ", ";
        }
        oss << m_conn->quote(it->first) << " = ";
        if (it->second.IsString()) {
            oss << "'" << m_conn->escape(it->second) << "'";
        } else {
            oss << (std::string)(it->second);
        }
    }
    // oss << build_join_sql();
    oss << build_where_sql();
    oss << build_other_sql();
    std::string sql = oss.str();
    m_conn->execute(sql);
}

template <typename T>
void Model<T>::insert(const std::vector<T>& rows) {
    if (rows.empty()) {
        return;
    }
    std::ostringstream oss;
    oss << "insert into " << m_conn->quote(table());
    oss << "(";
    for (auto it = rows[0].m_new_fields.begin();
         it != rows[0].m_new_fields.end();
         it++) {
        if (it != rows[0].m_new_fields.begin()) {
            oss << ",";
        }
        oss << m_conn->quote(it->first);
    }
    oss << ") values ";
    for (auto row = rows.begin(); row != rows.end(); row++) {
        if (row != rows.begin()) {
            oss << ",";
        }
        oss << "(";
        for (auto it = row->m_new_fields.begin(); it != row->m_new_fields.end();
             it++) {
            if (it != row->m_new_fields.begin()) {
                oss << ",";
            }
            if (it->second.IsString()) {
                oss << "'" << m_conn->escape(it->second) << "'";
            } else {
                oss << std::string(it->second);
            }
        }
        oss << ")";
    }
    std::string sql = oss.str();
    m_conn->insert(sql);
}

template <typename T>
void Model<T>::truncate() {
    std::ostringstream oss;
    oss << "truncate table " << m_conn->quote(table());
    std::string sql = oss.str();
    m_conn->execute(sql);
}

template <typename T>
int Model<T>::count(const std::string& field) {
    std::ostringstream oss;
    oss << "select count(" << field << ") from " << m_conn->quote(table());
    if (!m_alias.empty()) {
        oss << " as " << m_conn->quote(m_alias);
    }
    oss << build_join_sql();
    oss << build_where_sql();
    oss << build_other_sql();
    std::string sql = oss.str();
    auto one = m_conn->fetchone(sql);
    for (auto it = one.begin(); it != one.end(); it++) {
        return it->second;
    }
    return 0;
}

template <typename T>
double Model<T>::sum(const std::string& field) {
    std::string sql = build_simple_sql(field, "sum");
    auto one = m_conn->fetchone(sql);
    for (auto it = one.begin(); it != one.end(); it++) {
        return it->second;
    }
    return 0;
}

template <typename T>
double Model<T>::min(const std::string& field) {
    std::string sql = build_simple_sql(field, "min");
    auto one = m_conn->fetchone(sql);
    for (auto it = one.begin(); it != one.end(); it++) {
        return it->second;
    }
    return 0;
}

template <typename T>
double Model<T>::max(const std::string& field) {
    std::string sql = build_simple_sql(field, "max");
    auto one = m_conn->fetchone(sql);
    for (auto it = one.begin(); it != one.end(); it++) {
        return it->second;
    }
    return 0;
}

template <typename T>
double Model<T>::average(const std::string& field) {
    std::string sql = build_simple_sql(field, "avg");
    auto one = m_conn->fetchone(sql);
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
    auto one = m_conn->fetchone(sql);
    for (auto it = one.begin(); it != one.end(); it++) {
        total = it->second;
    }
    return total == 1;
}

template <typename T>
Value Model<T>::scalar(const std::string& field) {
    m_limit = 1;
    std::string sql = build_simple_sql(field);
    auto one = m_conn->fetchone(sql);
    return one[field];
}

template <typename T>
std::vector<Value> Model<T>::column(const std::string& field) {
    std::string sql = build_simple_sql(field);
    auto all = m_conn->fetchall(sql);
    std::vector<Value> result;
    for (auto it = all.begin(); it != all.end(); it++) {
        result.push_back((*it)[field]);
    }
    return result;
}



template <typename T>
T Model<T>::one() {
    m_limit = 1;
    T one(m_conn);
    one.m_old_fields = m_conn->fetchone(sql());
    return one;
}

template <typename T>
std::vector<T> Model<T>::all() {
    std::vector<T> all;
    std::vector<std::map<std::string, Value>> result = m_conn->fetchall(sql());
    for (auto it = result.begin(); it != result.end(); it++) {
        T one(m_conn);
        one.m_old_fields = (*it);
        all.push_back(one);
    }
    return all;
}

template <typename T>
Batch<T> Model<T>::batch(int size) {
    Batch<T> batch = m_conn->batch<T>(sql());
    batch.size(size);
    return batch;
}

} // namespace mysql

} // namespace zel