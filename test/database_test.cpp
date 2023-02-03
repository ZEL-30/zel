#include "mysql/_mysql.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>


using namespace zel::database;

TEST_CASE("testing Class mysql") {

    connection conn;

    // 登录数据库
    if (conn.connecttodb("127.0.0.1,root,19981110,data,3305", "utf8") != 0) {
        printf("connect database failed.\n%s\n", conn.m_cda.message);
        return;
    }
    printf("connect database ok.\n");

    char tablename[256] = "test_data";
    int state = 0;
    char primary_key[256] = "data_id";

    // 准备查询表的SQL语句
    sqlstatement stmt(&conn);
    stmt.prepare("select * from %s where state = %d order by %s desc limit 1",
                 tablename,
                 state,
                 primary_key);

    // 为SQL语句绑定输出语句
    int count = 12;
    char data_value[count][256];

    for (int i = 0; i < count; i++) {
        stmt.bindout(i + 1, data_value[i], 250);
    }

    // 执行SQL语句，一定要判断返回值，0-成功，其它-失败。
    if (stmt.execute() != 0) {
        printf(
            "stmt.execute() failed.\n%s\n%s\n", stmt.m_sql, stmt.m_cda.message);
        return;
    }

    // 获取数据库查询的结果集
    while (true) {
        memset(data_value, 0, sizeof(data_value));

        if (stmt.next() != 0)
            break;

        for (int i = 0; i < count; i++) {
            printf("%s ", data_value[i]);
        }
        printf("\n");
    }
}