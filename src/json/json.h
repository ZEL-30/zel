#pragma once

#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>

namespace zel {

namespace json {

class Json {

  public:
    enum Type {
        JSON_NULL = 0,
        JSON_BOOL,
        JSON_INT,
        JSON_DOUBLE,
        JSON_STRING,
        JSON_ARRAY,
        JSON_OBJECT
    };

    Json();
    Json(Type type);
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char* value);
    Json(const std::string& value);
    Json(const Json& other);
    ~Json();

    Type type() const;

    // static
    static Json const& null();

    /// @brief 序列化为 C++ string
    std::string str() const;

    const Json& get(int index) const;
    const Json& get(const char* key) const;
    const Json& get(const std::string& key) const;

    void set(const Json& other);
    void set(bool value);
    void set(int value);
    void set(double value);
    void set(const char* value);
    void set(const std::string& value);

    /// @brief 追加值到数组末尾
    void append(const Json& other);

    bool has(int index);
    bool has(const char* key);
    bool has(std::string& key);

    void remove(int index);
    void remove(const char* key);
    void remove(std::string& key);

    bool parse(const std::string& str);

    void copy(const Json& other);
    void clear();

    /// @brief 类型转换
    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    std::string asString() const;

    bool isNULL() const;
    bool isBool() const;
    bool isInt() const;
    bool isDouble() const;
    bool isString() const;
    bool isArray() const;
    bool isObject() const;

    operator bool();
    operator int();
    operator double();
    operator std::string();
    operator std::string() const;
    operator std::basic_string<char>::value_type*() const;

    /// @brief 访问一个数组元素（从零开始的索引）
    Json& operator[](int index);
    /// @brief
    /// 访问一个数组元素（从零开始的索引），如果没有该名称的成员则返回null
    const Json& operator[](int index) const;

    /// @brief 通过名称访问对象值，如果不存在则创建空成员
    Json& operator[](const char* key);
    /// @brief 通过名称访问对象值，如果没有该名称的成员则返回null
    const Json& operator[](const char* key) const;

    /// @brief 通过名称访问对象值，如果不存在则创建空成员
    Json& operator[](const std::string& key);
    /// @brief 通过名称访问对象值，如果没有该名称的成员则返回null
    const Json& operator[](const std::string& key) const;

    Json& operator=(const Json& other);
    Json& operator=(bool value);
    Json& operator=(int value);
    Json& operator=(double value);
    Json& operator=(const char* value);
    Json& operator=(const std::string& value);

    bool operator==(const Json& other);
    bool operator==(bool value);
    bool operator==(int value);
    bool operator==(double value);
    bool operator==(const char* value);
    bool operator==(const std::string& value);

    bool operator!=(const Json& other);
    bool operator!=(bool value);
    bool operator!=(int value);
    bool operator!=(double value);
    bool operator!=(const char* value);
    bool operator!=(const std::string& value);

    std::vector<Json>::iterator begin();
    std::vector<Json>::const_iterator begin() const;
    std::vector<Json>::iterator end();
    std::vector<Json>::const_iterator end() const;
    int size();
    bool empty();

  private:
    union Value {
        bool bool_;
        int int_;
        double double_;
        std::string* string_;
        std::vector<Json>* array_;
        std::map<std::string, Json>* object_;
    };

    Type type_;
    Value value_;
};

} // namespace json

} // namespace zel