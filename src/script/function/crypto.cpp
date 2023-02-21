// #include "crypto.h"

// namespace script {

// namespace function {

// Crypto::Crypto() {

//     memset(input_, 0, sizeof(input_));
//     memset(key_, 0, sizeof(key_));
//     memset(output_, 0, sizeof(output_));

//     GetModeType();
// }

// Crypto::~Crypto() {}

// CValue* Crypto::Cipher(std::vector<CValue*> v_args) {

//     char str_result[1024];
//     memset(str_result, 0, sizeof(str_result));

//     // 选择加密方式
//     switch (m_mode_type_[v_args[0]->value_]) {

//     case DES_ECB:
//         Run1Des(atoi(v_args[4]->value_),
//                 ECB,
//                 v_args[1]->value_,
//                 v_args[2]->value_,
//                 str_result,
//                 v_args[3]->value_);
//         break;

//     case DES_CBC:
//         Run1Des(atoi(v_args[4]->value_),
//                 CBC,
//                 v_args[1]->value_,
//                 v_args[2]->value_,
//                 str_result,
//                 v_args[3]->value_);
//         break;

//     case DES_EDE_CBC:
//         Run3Des(atoi(v_args[4]->value_),
//                 CBC,
//                 v_args[1]->value_,
//                 v_args[2]->value_,
//                 str_result,
//                 v_args[3]->value_);
//         break;

//     case DES_EDE:
//         Run3Des(atoi(v_args[4]->value_),
//                 ECB,
//                 v_args[1]->value_,
//                 v_args[2]->value_,
//                 str_result,
//                 v_args[3]->value_);
//         break;

//     default:
//         break;
//     }

//     return new CStringValue(str_result);
// }

// void Crypto::GetModeType() {

//     m_mode_type_["des-ede-cbc"] = DES_EDE_CBC;
//     m_mode_type_["des-ede"] = DES_EDE;
//     m_mode_type_["des-ecb"] = DES_ECB;
// }

// CValue* Crypto::RandomHex(std::vector<CValue*> v_args) {

//     char str_result[1024];
//     memset(str_result, 0, sizeof(str_result));
//     RandomHex(str_result, atoi(v_args[0]->value_));

//     return new CStringValue(str_result);
// }

// CValue* Crypto::TDesMac(std::vector<CValue*> v_args) {
//     char str_result[1024];
//     memset(str_result, 0, sizeof(str_result));
//     PBOC_MAC(v_args[0]->value_, v_args[1]->value_, str_result, v_args[2]->value_);
//     return new CStringValue(str_result);
// }

// int Crypto::Run1Des(int type,
//                      int mode,
//                      const char* str_input,
//                      const char* str_key,
//                      char* str_out,
//                      const char* str_ivec) {

//     // 将传入参数的ASCII码压缩为BCD码
//     ArgsToBcd(str_input, str_key, str_out, str_ivec);

//     ret_ = Run1Des(type, mode, input_, input_len_, key_, key_len_, output_, sizeof(output_), ivec_);

//     BcdToAsc(str_out, (char*)output_, input_len_);

//     return ret_;
// }

// int Crypto::Run1Des(int type,
//                      int mode,
//                      unsigned char* input,
//                      unsigned int in_len,
//                      unsigned char* key,
//                      unsigned int key_len,
//                      unsigned char* output,
//                      unsigned int out_len,
//                      unsigned char* ivec) {

//     // 初始化秘钥
//     DES_key_schedule key_schedule;
//     DES_set_key_unchecked((const_DES_cblock*)key, &key_schedule);

//     if (mode == ECB) {
//         for (int i = 0; i < in_len / 8; i++) {
//             memset(out_, 0, 8);
//             DES_ecb_encrypt(
//                 (const_DES_cblock*)(input + (i * 8)), (DES_cblock*)out_, &key_schedule, type);
//             memcpy((char*)output + i * 8, (const char*)out_, 8);
//         }
//     } else if (mode == CBC) {
//         DES_ncbc_encrypt(
//             (const unsigned char*)input, output, in_len, &key_schedule, (DES_cblock*)ivec, type);
//     } else {
//         return 0;
//     }

//     return 1;
// }

// int Crypto::Run3Des(int type,
//                      int mode,
//                      const char* str_input,
//                      const char* str_key,
//                      char* str_out,
//                      const char* str_ivec) {

//     // 将传入参数的ASCII码压缩为BCD码
//     ArgsToBcd(str_input, str_key, str_out, str_ivec);

//     ret_ = Run3Des(type, mode, input_, input_len_, key_, key_len_, output_, sizeof(output_), ivec_);
//     BcdToAsc(str_out, (char*)output_, input_len_);

//     return ret_;
// }

// int Crypto::Run3Des(int type,
//                      int mode,
//                      unsigned char* input,
//                      unsigned int in_len,
//                      unsigned char* key,
//                      unsigned int key_len,
//                      unsigned char* output,
//                      unsigned int out_len,
//                      unsigned char* ivec) {

//     // 初始化秘钥
//     switch (key_len) {

//     case KEY_LEN_8: {
//         DES_set_key_unchecked(block_key_, &key_schedule1_);
//         break;
//     }

//     case KEY_LEN_16: {
//         memcpy(block_key_, key + 0, 8);
//         DES_set_key_unchecked(block_key_, &key_schedule1_);
//         memcpy(block_key_, key + 8, 8);
//         DES_set_key_unchecked(block_key_, &key_schedule2_);
//         break;
//     }

//     case KEY_LEN_24: {
//         memcpy(block_key_, key + 0, 8);
//         DES_set_key_unchecked(block_key_, &key_schedule1_);
//         memcpy(block_key_, key + 8, 8);
//         DES_set_key_unchecked(block_key_, &key_schedule2_);
//         memcpy(block_key_, key + 16, 8);
//         DES_set_key_unchecked(block_key_, &key_schedule3_);
//         break;
//     }

//     default:
//         break;
//     }

//     if (mode == ECB) {

//         switch (key_len) {

//         case KEY_LEN_8: {
//             for (int i = 0; i < in_len / 8; i++) {
//                 memset(out_, 0, 8);
//                 DES_ecb3_encrypt((const_DES_cblock*)(input + (i * 8)),
//                                  (DES_cblock*)out_,
//                                  &key_schedule1_,
//                                  &key_schedule1_,
//                                  &key_schedule1_,
//                                  type);
//                 strncat((char*)output + i * 8, (const char*)out_, 8);
//             }
//             break;
//         }

//         case KEY_LEN_16: {
//             for (int i = 0; i < in_len / 8; i++) {
//                 memset(out_, 0, 8);
//                 DES_ecb3_encrypt((const_DES_cblock*)(input + (i * 8)),
//                                  (DES_cblock*)out_,
//                                  &key_schedule1_,
//                                  &key_schedule2_,
//                                  &key_schedule1_,
//                                  type);
//                 memcpy((char*)output + (i * 8), (const char*)out_, 8);
//             }
//             break;
//         }

//         case KEY_LEN_24: {
//             for (int i = 0; i < in_len / 8; i++) {
//                 memset(out_, 0, 8);
//                 DES_ecb3_encrypt((const_DES_cblock*)(input + (i * 8)),
//                                  (DES_cblock*)out_,
//                                  &key_schedule1_,
//                                  &key_schedule2_,
//                                  &key_schedule3_,
//                                  type);
//                 strncat((char*)output + i * 8, (const char*)out_, 8);
//             }
//             break;
//         }

//         default:
//             break;
//         }

//     } else if (mode == CBC) {

//         switch (key_len) {

//         case KEY_LEN_8: {
//             DES_ede3_cbc_encrypt((const unsigned char*)input,
//                                  output,
//                                  in_len,
//                                  &key_schedule1_,
//                                  &key_schedule1_,
//                                  &key_schedule1_,
//                                  (DES_cblock*)ivec,
//                                  type);
//             break;
//         }

//         case KEY_LEN_16: {
//             DES_ede3_cbc_encrypt((const unsigned char*)input,
//                                  output,
//                                  in_len,
//                                  &key_schedule1_,
//                                  &key_schedule2_,
//                                  &key_schedule1_,
//                                  (DES_cblock*)ivec,
//                                  type);
//             break;
//         }

//         case KEY_LEN_24: {
//             DES_ede3_cbc_encrypt((const unsigned char*)input,
//                                  output,
//                                  in_len,
//                                  &key_schedule1_,
//                                  &key_schedule2_,
//                                  &key_schedule3_,
//                                  (DES_cblock*)ivec,
//                                  type);
//             break;
//         }

//         default:
//             break;
//         }

//     } else {

//         return 0;
//     }

//     return 1;
// }

// int Crypto::PBOC_MAC(const char* str_input,
//                       const char* str_key,
//                       char* str_out,
//                       const char* str_ivec) {

//     // 将传入参数的ASCII码压缩为BCD码
//     ArgsToBcd(str_input, str_key, str_out, str_ivec);

//     unsigned char in_oxr[8], out[16];
//     memset(out, 0, sizeof(out));

//     // 准备工作
//     int x = input_len_ / 8; // 计算有多少个完整的块
//     int n = input_len_ % 8; // 计算最后一个块有几个字节
//     if (n != 0)             // y非0,则在其后补上0x00...
//     {
//         memset(&input_[x * 8 + n], 0x00, 8 - n);
//         input_[x * 8 + n] = 0x80;
//     } else {
//         memset(&input_[x * 8], 0x00, 8); // 如果最后一块长度是8个字节，则最后加80 00。。
//         input_[x * 8] = 0x80;
//     }

//     // 开始计算
//     int i = 0;
//     DataXOR(ivec_, input_, 8, in_oxr);

//     for (i = 1; i < x + 1; i++) {
//         if (Run1Des(ENCRYPT, ECB, in_oxr, 8, key_, key_len_, out, 8, nullptr) != 1)
//             return -1;
//         DataXOR(out, input_ + 8 * i, 8, in_oxr);
//     }
//     if (Run3Des(ENCRYPT, ECB, in_oxr, 8, key_, key_len_, out, 8, nullptr) != 1)
//         return -1;

//     BcdToAsc(str_out, (char*)out, 8);

//     return 1;
// }

// void Crypto::DataXOR(unsigned char* source, unsigned char* dest, int size, unsigned char* out) {
//     for (int i = 0; i < size; i++)
//         out[i] = dest[i] ^ source[i];
// }

// void Crypto::ArgsToBcd(const char* str_input,
//                         const char* str_key,
//                         char* str_out,
//                         const char* str_ivec) {

//     AscToBcd(input_, (unsigned char*)str_input, strlen(str_input));
//     input_len_ = strlen(str_input) / 2;

//     AscToBcd(key_, (unsigned char*)str_key, strlen(str_key));
//     key_len_ = strlen(str_key) / 2;

//     if (strcmp(str_ivec, "NULL") != 0) {
//         AscToBcd(ivec_, (unsigned char*)str_ivec, strlen(str_ivec));
//     }
// }

// void Crypto::RandomHex(char* str_hex, int len) {

//     // 生成 len 长度的随机数
//     int hex_len = len / 2;
//     unsigned char hex[hex_len] = {0}; // 用来接收产生的随机数
//     RAND_bytes(hex, hex_len);

//     // 转成16进制字符串表示
//     BcdToAsc(str_hex, (char*)hex, hex_len);
// }

// void Crypto::BcdToAsc(char* asc, char* bcd, int bcd_len) {
//     unsigned char ch1, ch2;
//     for (int i = 0; i < bcd_len; i++) {
//         ch1 = (bcd[i] & 0xF0) >> 4;
//         ch2 = bcd[i] & 0x0F;
//         ch1 += ((ch1 > 9) ? 0x37 : 0x30);
//         ch2 += ((ch2 > 9) ? 0x37 : 0x30);
//         asc[i * 2] = ch1;
//         asc[i * 2 + 1] = ch2;
//     }
// }

// void Crypto::AscToBcd(unsigned char* bcd, unsigned char* asc, int asc_len) {
//     unsigned char ch1, ch2;
//     for (int i = 0; i < asc_len / 2; i++) {
//         ch1 = asc[i * 2];
//         ch2 = asc[i * 2 + 1];
//         (ch1 >= 'a' && ch1 <= 'z') ? (ch1 -= 32) : (ch1);
//         (ch2 >= 'a' && ch2 <= 'z') ? (ch2 -= 32) : (ch2);
//         ch1 -= ((ch1 > '9') ? 0x37 : 0x30);
//         ch2 -= ((ch2 > '9') ? 0x37 : 0x30);
//         bcd[i] = (ch1 << 4) | ch2;
//     }
// }

// void Crypto::AscToHex(unsigned char* hex, unsigned char* asc, int asc_len) {
//     char ch1, ch2;
//     for (int i = 0; i < asc_len / 2; i++) {
//         ch1 = asc[i * 2];
//         ch2 = asc[i * 2 + 1];
//         if ((ch1 >= '0') && (ch1 <= '9'))
//             ch1 -= '0';
//         if ((ch2 >= '0') && (ch2 <= '9'))
//             ch2 -= '0';

//         if ((ch1 >= 'A') && (ch1 <= 'F'))
//             ch1 = ch1 - 'A' + 0x0A;
//         if ((ch2 >= 'A') && (ch2 <= 'F'))
//             ch2 = ch2 - 'A' + 0x0A;

//         if ((ch1 >= 'a') && (ch1 <= 'f'))
//             ch1 -= 'a' + 0x0A;
//         if ((ch2 >= 'a') && (ch2 <= 'f'))
//             ch2 -= 'a' + 0x0A;

//         hex[i] = (ch1 << 4) | ch2;
//     }
// }

// void Crypto::HexToAsc(char* asc, char* hex, int hex_len) {
//     unsigned char ch1, ch2;
//     int i = 0;
//     for (i = 0; i < hex_len; i++) {
//         ch1 = (hex[i] & 0xF0) >> 4;
//         ch2 = hex[i] & 0x0F;
//         ch1 += ((ch1 > 9) ? 0x37 : 0x30);
//         ch2 += ((ch2 > 9) ? 0x37 : 0x30);
//         asc[i * 2] = ch1;
//         asc[i * 2 + 1] = ch2;
//     }
//     asc[i * 2] = '\0';
// }

// } // namespace funciton

// } // namespace script