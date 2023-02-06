#include <mysql/database.h>
#include <utility/logger.h>

using namespace zel::utility;

namespace zel {

namespace mysql {

Database::Database() : m_conn(nullptr) {}

Database::Database(Connection* conn) : m_conn(conn) {}

Database::~Database() {}

bool Database::connect(const std::string& host,
                       int port,
                       const std::string& username,
                       const std::string& password,
                       const std::string& database,
                       const std::string& charset,
                       bool debug) {
    m_conn = new Connection();
    return m_conn->connect(
        host, port, username, password, database, charset, debug);
}

void Database::close() {
    if (m_conn != nullptr) {
        m_conn->close();
        delete m_conn;
    }
}

void Database::execute(const std::string& sql) { m_conn->execute(sql); }

std::vector<std::map<std::string, Value>>
Database::query(const std::string& sql) {
    return m_conn->fetchall(sql);
}

std::vector<std::string> Database::tables() { return m_conn->tables(); }

bool Database::exists(const std::string& table) {
    return m_conn->table_exists(table);
}

std::vector<std::map<std::string, Value>>
Database::schema(const std::string& table) {
    return m_conn->schema(table);
}

std::string Database::primary_key(const std::string& table) {
    return m_conn->primary_key(table);
}

std::string Database::escape(const std::string& str) {
    return m_conn->escape(str);
}

Connection* Database::operator()() { return m_conn; }

} // namespace mysql

} // namespace zel