#pragma once

#include "../interpreter/value.h"

#include <memory>
#include <vector>

namespace script {

namespace function {

/// @brief  自定义字符串操作类
class String {

  public:
    String();
    ~String();

    std::shared_ptr<interpreter::Value>
    mid(std::vector<std::shared_ptr<interpreter::Value>> v_args);

  private:
};

} // namespace function

} // namespace script