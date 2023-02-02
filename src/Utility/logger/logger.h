#pragma once

#include <fstream>
#include <string>

namespace zel {
namespace utility {

class CLogger {

  public:
    // 定义日志级别
    enum Level { DEBUG = 0, INFO, WARN, ERROR, FATAL, LEVEL_COUNT };

    static CLogger *Instance();

    void Open(const std::string &filename);

    void Close();

    void Log(Level level, const char* file, int line, const char* format, ...);

  private:
    CLogger();
    CLogger(const CLogger&);
    ~CLogger();

  private:
    std::string filename_; // 日志文件名
    std::ofstream fout_;   // 文件输入流
    static const char* level_[LEVEL_COUNT];
    static CLogger* instance_;
};

} // namespace utility
} // namespace zel