// #include "ast_node.h"

// #include <string>

// namespace zel {

// namespace interpreter {

// // std::string AstNode::str() const {
// //     // std::string str;

// //     // switch (type_) {

// //     // case APDU:
// //     // case STRING:
// //     // case APPEND:
// //     // case VARACCESS:
// //     // case VARASSIGN:
// //     // case FUNCTION:
// //     //     break;
// //     // }
// // };

// StringNode::StringNode(Token& token) {
//     token_ = token;
// }

// // AstNode StringNode::value() {}

// std::string StringNode::str() const {
//     std::string str;
//     str = "(" + token_.str() + ")";
//     return str;
// }

// StringNode::~StringNode() {}

// } // namespace interpreter

// } // namespace zel