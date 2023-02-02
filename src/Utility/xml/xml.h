#pragma once
#include "value/value.h"

#include <list>
#include <map>
#include <string>

namespace zel {

namespace utility {

class CXml {

  public:
    CXml();
    CXml(const char* name);
    CXml(const std::string& name);
    CXml(const CXml& other);
    CXml& operator=(const CXml& other);

    ~CXml();

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
    CValue& GetAttr(const std::string& key);

    /// @brief 设置节点属性
    /// @param key 属性名
    /// @param value 属性值
    void SetAttr(const std::string& key, const CValue& value);

    /// @brief 节点序列化
    /// @return std::string
    std::string AsString();

    void Clear();

    void Append(const CXml& child);

    CXml& operator[](int index);
    CXml& operator[](const char* name);
    CXml& operator[](const std::string& name);

    void Remove(int index);
    void Remove(const char* name);
    void Remove(const std::string& name);

    // 定义迭代器
    std::list<CXml>::iterator begin();
    std::list<CXml>::iterator end();
    std::list<CXml>::iterator erase(std::list<CXml>::iterator it);

    int size() const;

  private:
    std::string* name_;                    // 节点名称
    std::string* text_;                    // 节点内容
    std::map<std::string, CValue>* attrs_; // 节点属性
    std::list<CXml>* child_;               // 子节点
};

} // namespace utility

} // namespace zel