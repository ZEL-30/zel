#include "models/test_data.h"

#include <mysql/database.h>
using namespace zel::mysql;

#include <utility/ini_file.h>
#include <utility/logger.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <iostream>

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

    TestData test_data;
    test_data["state"] = 1;

    TestData(database)
        .where("state", 0)
        .order(ini["mysql"]["primary_key"].str() + " asc")
        .one()
        .update(test_data);

    // cout << one("ICCID").str() << endl;


    // // 原地修改
    // TestData(database)
    //     .where("state", 1)
    //     .order(ini["mysql"]["primary_key"].str() + " asc")
    //     .one()
    //     .update({{"state", 0}});

    database.close();
}