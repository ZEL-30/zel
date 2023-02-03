#include "inifile/ini_file.h"
#include "logger/logger.h"
#include "xml/xml.h"

#include <doctest/doctest.h>
#include <iostream>

using namespace zel::utility;
using namespace std;

TEST_CASE("testing Class Logger") {

    Logger::Instance()->Open("../log/test.log");
    Logger::Instance()->SetMax(200);

    debug("勇士总冠军");
    warn("warn message");
    info("info message");
    fatal("fatal message");
}

TEST_CASE("testing Class IniFile") {
    IniFile ini_file;
    ini_file.Load("../config/main.ini");

    const std::string& ip = ini_file["mysql"]["ip"];
    int port = ini_file["mysql"]["port"];

    std::cout << "ip: " << ip << "\nport: " << port << "\n";

    ini_file.Set("server", "timeout", 1000);
    ini_file.Set("server", "flag", true);

    bool b = ini_file.Has("server", "flag");
    bool w = ini_file.Has("mysql");

    ini_file.Remove("server", "flag");

    ini_file.Show();

    if (!ini_file.Save("../config/temp.ini"))
        printf("Save Failed!\n");
}

TEST_CASE("testing Class Xml") {

    Xml root1;
    root1.Load("../config/workflow.xml");

    int id  = root1[0].attr("name");
    cout << root1[0].name() << id << endl;

}