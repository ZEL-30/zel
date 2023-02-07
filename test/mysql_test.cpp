#include "models/test_data.h"
#include "utility/ini_file.h"
#include "utility/logger.h"

#include <mysql/connection.h>
#include <mysql/database.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

using namespace zel::mysql;
using namespace std;

TEST_CASE("testing Class mysql") {

    Logger::instance()->open("../log/main.log");

    IniFile ini;
    ini.load("../config/main.ini");

    Database database;

    database.connect(ini["mysql"]["ip"],
                     ini["mysql"]["port"],
                     ini["mysql"]["username"],
                     ini["mysql"]["password"],
                     ini["mysql"]["dbname"],
                     ini["mysql"]["charset"],
                     true);


    auto one = TestData(database).where("state", 0).order(ini["mysql"]["primary_key"].AsString() + " asc").one();

    cout << one.get("ICCID").str() << endl;

    database.close();

}