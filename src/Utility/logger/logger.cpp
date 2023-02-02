/// @file logger.cpp
/// @author ZEL (zel1362848545@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-02-01
/// @copyright Copyright (c) 2023 ZEL

#include "logger.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <sys/_types/_time_t.h>
#include <time.h>

namespace zel {
namespace utility {

const char* CLogger::level_[LEVEL_COUNT] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

CLogger* CLogger::instance_ = nullptr;

CLogger::CLogger() {}

CLogger::CLogger(const CLogger&) {}

CLogger::~CLogger() {}

CLogger* CLogger::Instance() {
    if (instance_ == nullptr)
        return new CLogger();
    return instance_;
}

void CLogger::Open(const std::string& filename) {

    filename_ = filename;

    // 打开文件
    fout_.open(filename, std::ios::app);

    if (fout_.fail()) {
        throw std::logic_error("open file failed " + filename);
    }
}

void CLogger::Close() { fout_.close(); }

void CLogger::Log(Level level, const char* file, int line, const char* format, ...) {

    if (fout_.fail()) {
        throw std::logic_error("open file failed " + filename_);
    }

    time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);

    const char* fmt = "%s %s %s:%d ";
    int size = snprintf(NULL, 0, fmt, timestamp, level_[level], file, line);
    if (size > 0) {
        char* buffer = new char[size + 1];
        snprintf(buffer, size + 1, fmt, timestamp, level_[level], file, line);
        buffer[size] = '\0';

        std::cout << buffer << std::endl;
        fout_ << buffer;

    fout_.flush();
        delete[] buffer;
    }

}

} // namespace utility
} // namespace zel