#pragma once

#include "_mysql.h"

#include <string>
#include <vector>

namespace zel {

namespace database {

/// @brief 获取表全部的列和主键列信息的类
class CTables {

  public:
    CTables();

    /// @brief 成员变量初始化
    /// @return true
    /// @return false
    bool InitData();

    /// @brief 获取指定表的全部字段信息
    /// @param conn
    /// @param tablename
    /// @return true
    /// @return false
    bool AllCols(connection* conn, const char* tablename);

    // 获取指定表的主键字段信息
    bool PkCols(connection* conn, const char* tablename);

    ~CTables();

  public:
    int all_count_;       // 全部字段的个数
    int pk_count_;        // 主键字段的个数
    int max_collen_;      // 全部列中最大的长度
    char all_cols_[3001]; // 全部的字段, 已字符串存放, 中间使用半角的逗号分隔
    char pk_cols_[301]; // 全部的主键字段, 已字符串存放, 中间使用半角的逗号分隔

    /// @brief 表的列(字段)信息的结构体
    struct SColumns {
        char col_name[31];  // 列名
        char data_type[31]; // 列的数据类型, 分为number、date和char三大类
        int col_len; // 列的长度, number固定20, date固定19, char的长度由表结构决定
        int pk_seq; // 如果列是主键的字段, 存放主键字段的顺序, 从1开始, 不是主键取值0
    };

    std::vector<struct SColumns> v_all_cols_; // 存放全部字段信息的容器
    std::vector<struct SColumns> v_pk_cols_;  // 存放主键字段信息的容器

  private:
};

} // namespace database

} // namespace zel
