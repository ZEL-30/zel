#include "mysql/connection.h"
#include "mysql/database.h"

#include <mysql/model.h>

class TestData : public zel::mysql::Model<TestData> {

  public:
    TestData() : Model() {}
    TestData(zel::mysql::Database& db) : Model(db()) {}
    TestData(zel::mysql::Connection* conn) : Model(conn) {}
    ~TestData() {}

    string table() const { return "test_data"; }

    string primary_key() const { return "data_id"; }
};