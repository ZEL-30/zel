#include <iostream>
using namespace std;

#include <data_stream.h>
using namespace zel::serialize;

int main() {

    DataStream ds;

    string n = "zel";

    ds << n;

    ds.show();

    return 0;
}