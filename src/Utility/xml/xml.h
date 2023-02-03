#pragma once
#include "value/value.h"

#include <list>
#include <map>
#include <string>

namespace zel {

namespace utility {

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
    std::string GetName() const;

    /// @brief 设置节点名称
    /// @param name
    void SetName(const std::string& name);

    /// @brief 获取节点内容
    /// @return std::string
    std::string GetText() const;

    /// @brief 设置节点内容
    /// @param text
    void SetText(const std::string& text);

    /// @brief 获取节点属性
    /// @param key 属性名
    /// @return std::string 属性值
    Value& GetAttr(const std::string& key);

    /// @brief 设置节点属性
    /// @param key 属性名
    /// @param value 属性值
    void SetAttr(const std::string& key, const Value& value);

    /// @brief 节点序列化
    /// @return std::string
    std::string AsString();

    void Clear();

    void Append(const Xml& child);

    Xml& operator[](int index);
    Xml& operator[](const char* name);
    Xml& operator[](const std::string& name);

    void Remove(int index);
    void Remove(const char* name);
    void Remove(const std::string& name);

    // 定义迭代器
    std::list<Xml>::iterator begin();
    std::list<Xml>::iterator end();
    std::list<Xml>::iterator erase(std::list<Xml>::iterator it);

    int size() const;

  private:
    std::string* name_;                    // 节点名称
    std::string* text_;                    // 节点内容
    std::map<std::string, Value>* attrs_; // 节点属性
    std::list<Xml>* child_;               // 子节点
};

} // namespace utility

} // namespace zel