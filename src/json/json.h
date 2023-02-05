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
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char* value);
    Json(const std::string& value);
    Json(Type type);
    Json(const Json& other);
    ~Json();

    /// @brief 序列化为 C++ string
    std::string str() const;

    /// @brief 序列化为 C char*
    const std::basic_string<char>::value_type* c_str() const;

    void Append(const Json& other);

    bool Has(int index);
    bool Has(const char* key);
    bool Has(std::string& key);

    void Remove(int index);
    void Remove(const char* key);
    void Remove(std::string& key);


    bool Parse(const std::string& str);


    void Copy(const Json& other);
    void Clear();

    operator bool();
    operator int();
    operator double();
    operator std::string();
    operator std::string() const;
    operator std::basic_string<char>::value_type*() const;

    bool AsBool() const;
    int AsInt() const;
    double AsDouble() const;
    std::string AsString() const;

    Json& operator[](int index);
    Json& operator[](const char* key);
    Json& operator[](const std::string& key);

    Json& operator=(const Json& other);
    bool operator==(const Json& other);
    bool operator!=(const Json& other);

    bool IsNULL() const;
    bool IsBool() const;
    bool IsInt() const;
    bool IsDouble() const;
    bool IsString() const;
    bool IsArray() const;
    bool IsObject() const;

    std::vector<Json>::iterator begin();
    std::vector<Json>::iterator end();
    int size();

  private:
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