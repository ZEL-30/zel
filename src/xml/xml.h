#pragma once
#include "utility/value.h"

#include <list>
#include <map>
#include <sstream>
#include <string>

namespace zel {
namespace xml {

class Xml {
  public:
    Xml();
    Xml(const char* name);
    Xml(const std::string& name);
    Xml(const Xml& other);
    Xml& operator=(const Xml& other);

    ~Xml();

    /// @brief 获取节点名称
    /// @return std::string
    std::string name() const;

    /// @brief 设置节点名称
    /// @param name
    void name(const std::string& name);

    /// @brief 获取节点内容
    /// @return std::string
    std::string text() const;

    /// @brief 设置节点内容
    /// @param text
    void text(const std::string& text);

    /// @brief 获取节点属性
    /// @param key 属性名
    /// @return std::string 属性值
    zel::utility::Value& attr(const std::string& key);

    /// @brief 设置节点属性
    /// @param key 属性名
    /// @param value 属性值
    void attr(const std::string& key, const zel::utility::Value& value);

    /// @brief 节点序列化 C++ string
    std::string str() const;

    // /// @brief 序列化为 C char*
    // const std::basic_string<char>::value_type* c_str() const;

    void clear();

    void append(const Xml& child);

    void remove(int index);
    void remove(const char* name);
    void remove(const std::string& name);

    bool load(const std::string& filename);
    bool save(const std::string& filename);
    bool parse(const std::string& str);

    Xml& operator[](int index);
    Xml& operator[](const char* name);
    Xml& operator[](const std::string& name);

    // 定义迭代器
    std::list<Xml>::iterator begin();
    std::list<Xml>::iterator end();
    std::list<Xml>::iterator erase(std::list<Xml>::iterator it);

    int size() const;

  private:
    std::string* name_;                                 // 节点名称
    std::string* text_;                                 // 节点内容
    std::map<std::string, zel::utility::Value>* attrs_; // 节点属性
    std::list<Xml>* child_;                             // 子节点
};

} // namespace xml
} // namespace zel