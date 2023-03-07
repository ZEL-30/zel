#pragma once

#include <string>
#include <vector>

namespace zel {

namespace serialize {

class DataStream {

  public:
    DataStream();
    ~DataStream();

    void show() const;

    void write(bool value);
    void write(char value);
    void write(int32_t value);
    void write(int64_t value);
    void write(float value);
    void write(double value);
    void write(const std::string& value);
    void write(const char* value);
    void write(const char* data, int len);

    DataStream& operator<<(bool vlaue);
    DataStream& operator<<(char vlaue);
    DataStream& operator<<(int32_t vlaue);
    DataStream& operator<<(int64_t vlaue);
    DataStream& operator<<(float vlaue);
    DataStream& operator<<(double vlaue);
    DataStream& operator<<(const char* vlaue);
    DataStream& operator<<(const std::string& vlaue);

  private:
    void reserve(int len);

  public:
    enum Type { BOOL, CHAR, INT32, INT64, FLOAT, DOUBLE, STRING, VECTOR, LIST, MAP, SET, CUSTOM };

  private:
    std::vector<char> buf_;
};

} // namespace serialize

} // namespace zel