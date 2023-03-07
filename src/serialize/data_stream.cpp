#include "data_stream.h"

#include <iostream>

namespace zel {

namespace serialize {

DataStream::DataStream() {}

DataStream::~DataStream() {}

void DataStream::show() const {
    int size = buf_.size();

    int i = 0;
    while (i < size) {

        switch ((Type)buf_[0]) {

        case BOOL: {
            std::string data = (int)buf_[++i] == 0 ? "false" : "true";
            std::cout << "data: " << data.c_str();
            ++i;
            break;
        }

        case CHAR: {
            std::cout << "data: " << buf_[++i];
            ++i;
            break;
        }

        case INT32: {
            std::cout << "data: " << *((int32_t*)(&buf_[++i]));
            i += 4;
            break;
        }
        case INT64: {
            std::cout << "data: " << ((int64_t*)(&buf_[++i]));
            i += 8;
            break;
        }

        case FLOAT: {
            std::cout << "data: " << *((float*)(&buf_[++i]));
            i += 4;
            break;
        }
        case DOUBLE: {
            std::cout << "data: " << *((double*)(&buf_[++i]));
            i += 8;
            break;
        }
        case STRING: {
            if ((Type)buf_[++i] == Type::INT32) {
                int len = *((int32_t*)(&buf_[++i]));
                i += 4;
                std::cout << "data: " << std::string(&buf_[i], len);
                i += len;
            } else {
                throw std::logic_error("parse string error");
            }
            break;
        }
        case VECTOR:
        case LIST:
        case MAP:
        case SET:
        case CUSTOM:
            break;
        }
    }

    std::cout << " size: " << size << std::endl;
}

void DataStream::write(bool value) {
    // 编码 bool 类型 -> byte: 2 -> format: Type(1) + Value(1)

    // Type
    char type = Type::BOOL;
    write((char*)&type, sizeof(char));

    // Value
    write((char*)&value, sizeof(char));
}

void DataStream::write(char value) {
    // 编码 char 类型 -> byte: 2 -> format: Type(1) + Value(1)

    // Type
    char type = Type::CHAR;
    write((char*)&type, sizeof(char));

    // Value
    write((char*)&value, sizeof(char));
}

void DataStream::write(int32_t value) {
    // 编码 int32_t 类型 -> byte: 5 -> format: Type(1) + Value(4)

    // Type
    char type = Type::INT32;
    write((char*)&type, sizeof(char));

    // Value
    write((char*)&value, sizeof(int32_t));
}

void DataStream::write(int64_t value) {
    // 编码 int64_t 类型 -> byte: 9 -> format: Type(1) + Value(8)

    // Type
    char type = Type::INT64;
    write((char*)&type, sizeof(char));

    // Value
    write((char*)&value, sizeof(int64_t));
}

void DataStream::write(float value) {
    // 编码 float 类型 -> byte: 5 -> format: Type(1) + Value(4)

    // Type
    char type = Type::FLOAT;
    write((char*)&type, sizeof(char));

    // Value
    write((char*)&value, sizeof(float));
}

void DataStream::write(double value) {
    // 编码 double 类型 -> byte: 5 -> format: Type(1) + Value(4)

    // Type
    char type = Type::DOUBLE;
    write((char*)&type, sizeof(char));

    // Value
    write((char*)&value, sizeof(double));
}

void DataStream::write(const char* data, int len) {
    reserve(len);
    int size = buf_.size();
    buf_.resize(size + len);
    std::memcpy(&buf_[size], data, len);
}

void DataStream::write(const char* value) {
    // 编码 string 类型 -> byte: 可变长度 -> format: Type(1) + Length(5) + Value(变长)

    // Type
    char type = Type::STRING;
    write((char*)&type, sizeof(char));

    // Length
    int len = strlen(value);
    write(len);

    // Value
    write(value, len);
}

void DataStream::write(const std::string& value) { write(value.c_str()); }

DataStream& DataStream::operator<<(bool value) {
    write(value);
    return *this;
}

DataStream& DataStream::operator<<(char value) {
    write(value);
    return *this;
}

DataStream& DataStream::operator<<(int32_t value) {
    write(value);
    return *this;
}

DataStream& DataStream::operator<<(int64_t value) {
    write(value);
    return *this;
}

DataStream& DataStream::operator<<(float value) {
    write(value);
    return *this;
}

DataStream& DataStream::operator<<(double value) {
    write(value);
    return *this;
}

DataStream& DataStream::operator<<(const char* value) {
    write(value);
    return *this;
}

DataStream& DataStream::operator<<(const std::string& value) {
    write(value);
    return *this;
}

void DataStream::reserve(int len) {
    int size = buf_.size();
    int cap = buf_.capacity();

    if (size + len > cap) {
        while (size + len > cap) {
            if (cap == 0)
                cap = 1;
            else
                cap *= 2;
        }
        buf_.reserve(cap);
    }
}

} // namespace serialize

} // namespace zel