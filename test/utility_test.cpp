#include "utility/ini_file.h"
#include "utility/logger.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <iostream>

using namespace zel::utility;
using namespace std;

TEST_CASE("testing Class Logger") {

    Logger::Instance()->Open("/Users/zel/Workspaces/C++/zel//log/test.log");
    Logger::Instance()->max(10000);

    log_debug("勇士总冠军");
    log_warn("warn message");
    log_info("info message");
    log_fatal("fatal message");
}

TEST_CASE("testing Class IniFile") {
    IniFile ini_file;
    ini_file.Load("/Users/zel/Workspaces/C++/zel/config/main.ini");

    const std::string& ip = ini_file["mysql"]["ip"];
    int port = ini_file["mysql"]["port"];

    std::cout << "ip: " << ip << "\nport: " << port << "\n";

    // cout << ini_file.Str() << endl;
    cout << ini_file["mysql"]["tablename"].str() << endl;
    printf("%s\n", ini_file["mysql"]["tablename"].c_str());

    char* dbname = ini_file["mysql"]["dbname"];
    printf("dbname: %s\n", dbname);
    // ini_file.Set("server", "timeout", 1000);
    // ini_file.Set("server", "flag", true);

    // bool b = ini_file.Has("server", "flag");
    // bool w = ini_file.Has("mysql");

    // ini_file.Remove("server", "flag");

    // ini_file.Show();

    // if (!ini_file.Save("../config/temp.ini"))
    //     printf("Save Failed!\n");
}
