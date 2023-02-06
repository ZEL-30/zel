#include "models/test_data.h"
#include "utility/logger.h"

#include <mysql/connection.h>
#include <mysql/database.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

using namespace zel::mysql;
using namespace std;

TEST_CASE("testing Class mysql") {

    Logger::Instance()->Open("../log/main.log");

    Database database;
    database.connect(
        "127.0.0.1", 3305, "root", "19981110", "data", "utf8", true);

    auto all = TestData(database).where("state", 0).all();

    for (auto one : all) {
        cout << one.str() << endl;
    }
}