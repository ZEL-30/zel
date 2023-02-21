#include "string.h"

namespace script {

namespace function {

String::String() {}

String::~String() {}

std::shared_ptr<interpreter::Value> mid(std::vector<std::shared_ptr<interpreter::Value>> v_args) {

    int start_pos = atoi(v_args[1]->value().string_->c_str());
    int variable_len = atoi(v_args[2]->value().string_->c_str());

    std::string str_result = v_args[0]->value().string_->substr(start_pos, variable_len);

    return std::make_shared<interpreter::Value>(str_result);
}

} // namespace function
} // namespace script