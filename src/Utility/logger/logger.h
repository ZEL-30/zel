#pragma once

#include <algorithm>
#include <fstream>
#include <string>

namespace zel {
namespace utility {

#define debug(format, ...)                                                                         \
    CLogger::Instance()->Log(CLogger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define info(format, ...)                                                                          \
    CLogger::Instance()->Log(CLogger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define warn(format, ...)                                                                          \
    CLogger::Instance()->Log(CLogger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define error(format, ...)                                                                         \
    CLogger::Instance()->Log(CLogger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define fatal(format, ...)                                                                         \
    CLogger::Instance()->Log(CLogger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

class CLogger {

  public:
    // 定义日志级别
    enum Level { DEBUG = 0, INFO, WARN, ERROR, FATAL, LEVEL_COUNT };

    static CLogger* Instance();

    /// @brief 打开日志文件
    /// @param filename 日志文件名
    void Open(const std::string& filename);

    /// @brief 关闭日志文件
    void Close();

    /// @brief 记录日志
    /// @param level 日志级别
    /// @param file 当前文件名
    /// @param line 当前文件行数
    /// @param format 类似于 printf
    /// @param ...
    void Log(Level level, const char* file, int line, const char* format, ...);

    /// @brief 设置日志文件最大长度
    /// @param bytes 日志文件最大长度
    void SetMax(int bytes);

    /// @brief 设置日志级别
    /// @param level 日志级别
    void SetLevel(Level level);

  private:
    CLogger();
    CLogger(const CLogger&);
    ~CLogger();

    /// @brief 日志翻转
    void Rotate();

  private:
    std::string filename_; // 日志文件名
    std::ofstream fout_;   // 文件输入流
    int max_;              // 日志文件的最大长度
    int len_;              // 当前文件长度
    Level level_;          // 日志文件级别
    static const char* s_level_[LEVEL_COUNT];
    static CLogger* instance_;
};

} // namespace utility
} // namespace zel