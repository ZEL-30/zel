/// @file _tools.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-01-23
///
/// @copyright Copyright (c) 2023 ZEL
///
#include "tables.h"

namespace zel {

namespace database {

CTables::CTables() {
    // 调用成员变量初始化函数
    InitData();
}

bool CTables::InitData() {
    all_count_ = 0;
    pk_count_ = 0;
    max_collen_ = 0;
    memset(all_cols_, 0, sizeof(all_cols_));
    memset(pk_cols_, 0, sizeof(pk_cols_));
    v_all_cols_.clear();
    v_pk_cols_.clear();

    return true;
}

bool CTables::AllCols(connection* conn, const char* tablename) {
    all_count_ = 0;
    v_all_cols_.clear();
    memset(all_cols_, 0, sizeof(all_cols_));

    struct SColumns st_columns;

    sqlstatement stmt;
    stmt.connect(conn);
    stmt.prepare(
        "select lower(column_name), lower(data_type), character_maximum_length \ 
                  from information_schema.COLUMNS where table_name = :1");
    stmt.bindin(1, (char*)tablename, 30);
    stmt.bindout(1, st_columns.col_name, 30);
    stmt.bindout(2, st_columns.data_type, 30);
    stmt.bindout(3, &st_columns.col_len);

    // 执行SQL语句
    if (stmt.execute() != 0)
        return false;

    while (true) {
        memset(&st_columns, 0, sizeof(struct SColumns));

        if (stmt.next() != 0)
            break;

        // char类型
        if (strcmp(st_columns.data_type, "varchar") == 0)
            strcpy_s(st_columns.data_type, "char");
        if (strcmp(st_columns.data_type, "char") == 0)
            strcpy_s(st_columns.data_type, "char");

        // time类型
        if (strcmp(st_columns.data_type, "datetime") == 0)
            strcpy_s(st_columns.data_type, "date");
        if (strcmp(st_columns.data_type, "timestamp") == 0)
            strcpy_s(st_columns.data_type, "date");

        // number类型
        if (strcmp(st_columns.data_type, "tinyint") == 0)
            strcpy_s(st_columns.data_type, "number");
        if (strcmp(st_columns.data_type, "smallint") == 0)
            strcpy_s(st_columns.data_type, "number");
        if (strcmp(st_columns.data_type, "mediumint") == 0)
            strcpy_s(st_columns.data_type, "number");
        if (strcmp(st_columns.data_type, "int") == 0)
            strcpy_s(st_columns.data_type, "number");
        if (strcmp(st_columns.data_type, "integer") == 0)
            strcpy_s(st_columns.data_type, "number");
        if (strcmp(st_columns.data_type, "numeric") == 0)
            strcpy_s(st_columns.data_type, "number");
        if (strcmp(st_columns.data_type, "decimal") == 0)
            strcpy_s(st_columns.data_type, "number");
        if (strcmp(st_columns.data_type, "float") == 0)
            strcpy_s(st_columns.data_type, "number");
        if (strcmp(st_columns.data_type, "double") == 0)
            strcpy_s(st_columns.data_type, "number");
        if (strcmp(st_columns.data_type, "bigint") == 0)
            strcpy_s(st_columns.data_type, "number");

        // 如果业务有需要, 可以修改上面的代码, 增加对更多数据类型的支持
        // 如果字段的数据类型不在上面列出来的, 那么忽略它
        if (strcmp(st_columns.data_type, "date") != 0 &&
            strcmp(st_columns.data_type, "char") != 0 &&
            strcmp(st_columns.data_type, "number") != 0)
            continue;

        // 如果字段类型是date, 把长度设置为19, yyyy-mm-dd hh:mi:ss
        if (strcmp(st_columns.data_type, "date") == 0)
            st_columns.col_len = 19;

        // 如果字段类型是number, 把长度设置为20
        if (strcmp(st_columns.data_type, "number") == 0)
            st_columns.col_len = 20;

        // 拼接字符串的字段名
        strcat(all_cols_, st_columns.col_name);
        strcat(all_cols_, ",");

        v_all_cols_.push_back(st_columns);

        if (max_collen_ < st_columns.col_len)
            max_collen_ = st_columns.col_len;
        all_count_++;
    }

    // 删除m_allcols最后一个逗号
    if (all_count_ > 0)
        all_cols_[strlen(all_cols_) - 1] = '\0';

    return true;
}

bool CTables::PkCols(connection* conn, const char* tablename) {

    pk_count_ = 0;
    memset(pk_cols_, 0, sizeof(pk_cols_));
    v_pk_cols_.clear();

    struct SColumns st_columns;

    sqlstatement stmt;
    stmt.connect(conn);
    stmt.prepare(
        "select lower(column_name), seq_in_index \
                  from information_schema.STATISTICS \
                  where table_name = :1 and index_name = 'primary' order by seq_in_index");
    stmt.bindin(1, (char*)tablename, 30);
    stmt.bindout(1, st_columns.col_name, 30);
    stmt.bindout(2, &st_columns.pk_seq);

    // 执行SQL语句
    if (stmt.execute() != 0)
        return false;

    while (true) {
        memset(&st_columns, 0, sizeof(struct SColumns));

        // 获取结果集
        if (stmt.next() != 0)
            break;

        strcat(pk_cols_, st_columns.col_name);
        strcat(pk_cols_, ",");

        v_pk_cols_.push_back(st_columns);

        pk_count_++;
    }

    // 删除m_pkcols最后一个多余的逗号
    if (pk_count_ > 0)
        pk_cols_[strlen(pk_cols_) - 1] = 0;
    return true;
}

CTables::~CTables() {}

} // namespace database

} // namespace zel
