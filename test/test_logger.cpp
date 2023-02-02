#include "logger/logger.h"

using namespace zel::utility;

int main() {

    CLogger::Instance()->Open("./test.log");

    CLogger::Instance()->Log(CLogger::DEBUG, __FILE__, __LINE__, "勇士总冠军");

    return 0;
}