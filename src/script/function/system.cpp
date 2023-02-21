// #include "apdu.h"

// namespace script {

// namespace function {

// System::System(char* apdu, CReaderFactory* ReaderFac) {

//     strcpy_s(apdu_, apdu);
//     ReaderFac_ = ReaderFac;
//     memset(response_, 0, sizeof(response_));
//     memset(expected_, 0, sizeof(expected_));
// }

// System::~System() {}

// void System::GetApduType() {

//     m_apdu_type_["reset card"] = RST;
//     m_apdu_type_["PTS"] = PTS;
// }

// CValue* System::Execute() {

//     GetApduType();

//     // 根据不同类型的APDU执行不同的指令
//     switch (m_apdu_type_[apdu_]) {

//     case APDU:
//         SendApdu();
//         break;

//     case RST:
//         ResetCard();
//         break;

//     case PTS:
//         break;

//     default:
//         break;
//     }

//     return new CStringValue(response_);
// }

// CValue* System::Compare(std::vector<CValue*> v_expected) {

//     v_expected_ = v_expected;

//     // 获取期望值字符串
//     GetStrExpect();

//     // 比对相应值和期望值
//     compare_ = CompareApdu();

//     return new SystemValue(apdu_, response_, expected_, compare_);
// }

// void System::SendApdu() {

//     int com_len = strlen(apdu_) / 2, ret_len = 256;
//     unsigned char* command = (unsigned char*)malloc(com_len);

//     CCrypto Crypto;
//     Crypto.AscToHex(command, (unsigned char*)apdu_, strlen(apdu_));

//     // // 打印发送内容
//     // printf("SCardTransmit send: ");
//     // for(int i=0; i < com_len; i++) {
//     //     printf("%02x", command[i]);
//     // }
//     // printf("\n");

//     // 发送指令
//     char tmp_response[256];
//     memset(tmp_response, 0, sizeof(tmp_response));
//     int ret = ReaderFac_->CardApdu(command, com_len, tmp_response, &ret_len);
//     if (ret == -1)
//         strcpy_s(response_, "data failed\n");
//     else
//         // 将返回值转成ASCII码
//         Crypto.BcdToAsc(response_, tmp_response, ret_len);

//     // 释放指令
//     free(command);
// }

// void System::ResetCard() {

//     char state[256];
//     memset(state, 0, sizeof(state));
//     char protocol[256];
//     memset(protocol, 0, sizeof(protocol));
//     char tmp_response[256];
//     memset(tmp_response, 0, sizeof(tmp_response));
//     int atr_len = 32;

//     ReaderFac_->CardReset(tmp_response, &atr_len, state, protocol);

//     // 把ATR值转成ASCII码
//     CCrypto Crypto;
//     Crypto.BcdToAsc(response_, tmp_response, atr_len);
// }

// bool System::CompareApdu() {
//     char tmp_response[256]; // 定义用于比对的响应值

//     // 处理响应值
//     for (int i = 0; i < v_expected_.size(); i++) {
//         memset(tmp_response, 0, sizeof(tmp_response));

//         // 1. 处理带 " * "
//         if (strstr(v_expected_[i]->value_, "*") != nullptr) {

//             // SW值
//             char* sw = (char*)response_ + strlen(response_) - 4;
//             strcat(tmp_response, "*");
//             strcat(tmp_response, sw);

//             // 2.处理带 " XX "
//         } else if (strstr(v_expected_[i]->value_, "XX") != nullptr) {

//             strcpy_s(tmp_response, response_);
//             char* pos = (char*)v_expected_[i]->value_;
//             int len = 0;

//             while (true) {
//                 pos = strstr(pos, "XX");
//                 if (pos == nullptr)
//                     break;

//                 len = pos - (char*)v_expected_[i]->value_;
//                 strncpy(tmp_response + len, (char*)v_expected_[i]->value_ + len, 2);

//                 pos += 2;
//             }

//         } else {
//             strcpy_s(tmp_response, response_);
//         }

//         // 用于测试
//         // printf("response: \t%s\ntmp_response: \t%s\nv_args_: \t%s\n\n", response_, tmp_response,
//         // v_expected_[i]->value_);

//         // 对比期望值和响应值, 成功返回1
//         if (strcmp(v_expected_[i]->value_, tmp_response) == 0)
//             return true;
//     }

//     return false;
// }

// void System::GetStrExpect() {

//     char temp[256];
//     strcat(expected_, "(");
//     for (int i = 0; i < v_expected_.size(); i++) {
//         memset(temp, 0, sizeof(temp));
//         sprintf(temp, "%s", v_expected_[i]->value_);
//         if (i != v_expected_.size() - 1)
//             strcat(temp, ", ");

//         strcat(expected_, temp);
//     }
//     strcat(expected_, ")");
// }


// } // namespace function

// } // namespace script