// #pragma once

// #include "token.h"

// #include <string>
// #include <vector>

// namespace zel {

// namespace interpreter {

// class AstNode {

//   public:
//     enum Type {
//         LB_RST,
//         LB_POWERON,
//         LB_POWEROFF,
//         LB_APDU,
//         LB_READERSET,
//         LB_FUNCTION,
//         LB_SYNTAX,
//         LB_COMMENT,
//         LB_BLANK,
//         LB_GLOBAL,
//         LB_UNKNOW
//     };

//     // virtual AstNode value();
//     // virtual Type type();

//     virtual std::string str() const = 0;

//   private:
//     struct Apdu {
//         Token adpu;
//         std::string response;
//         std::vector<Token> v_expects;
//     };

//     struct Functoin {
      
//     };

//     union Value {
//         Apdu* apdu_;
//     };

//     Type type_;
//     Value value_;
// };

// } // namespace interpreter

// } // namespace zel