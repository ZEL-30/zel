#pragma once

#include "value/value.h"

#include <string>

namespace zel {

namespace utility {

class IniFile {

  public:
    IniFile();

    /// @brief 加载配置文件
    /// @param filename 配置文件名
    bool Load(const std::string& filename);

    /// @brief 保存配置文件
    /// @param filename 配置文件名
    bool Save(const std::string& filename);

    /// @brief 打印配置文件内容
    void Show();

    /// @brief 得到 section 中 key 的值
    /// @param section 段名
    /// @param key key名
    /// @return Value& key的值
    Value& Get(const std::string& section, const std::string& key);

    /// @brief 设置 key 的值
    /// @param section 需要设置的段名
    /// @param key 需要设置的 key
    /// @param value 需要设置的 value
    void Set(const std::string& section, const std::string& key, const Value& value);

    /// @brief 判断 seciton 中有没有某个 secition or key
    /// @param section
    /// @param key 需要判断的可以
    bool Has(const std::string& section);
    bool Has(const std::string& section, const std::string& key);

    /// @brief 删除某个段 或 某个key
    void Remove(const std::string& section);
    void Remove(const std::string& section, const std::string& key);

    /// @brief 清空整个配置文件
    void Clear();

    /// @brief 重载[]
    std::map<std::string, Value>& operator[](const std::string& section);

    ~IniFile();

  private:
    /// @brief 去除字符串前后的空格、换行、回车
    /// @param s 待处理的字符串
    /// @return std::string 处理后的字符串
    std::string Trim(std::string s);

    std::string Str();

  private:
    std::string filename_;
    std::map<std::string, std::map<std::string, Value>> m_sections_;
};
} // namespace utility

} // namespace zel
