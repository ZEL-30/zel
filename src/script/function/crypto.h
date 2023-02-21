// /*
//  * 文件: des_openssl.h
//  * 功能: Des算法类 (使用OpenSSL)
//  * 作者: ZEL
//  */

// #pragma once
// #include "../interpreter/value.h"

// #include <openssl/des.h>
// #include <openssl/rand.h>
// #include <stdio.h>
// #include <string.h>
// #include <memory>
// #include <vector>

// namespace script {

// namespace function {

// #define ENCRYPT 1 // 加密 1
// #define DECRYPT 0 // 解密 0

// #define ECB 0
// #define CBC 1

// #define KEY_LEN_8  8
// #define KEY_LEN_16 16
// #define KEY_LEN_24 24

// /// @brief 算法类
// class Crypto {

//   public:
//     Crypto();
//     ~Crypto();

//     std::shared_ptr<interpreter::Value> cipher(std::vector<std::shared_ptr<interpreter::Value>> v_args);
//     std::shared_ptr<interpreter::Value> randomHex(std::vector<std::shared_ptr<interpreter::Value>> v_args);
//     std::shared_ptr<interpreter::Value> tdesMac(std::vector<std::shared_ptr<interpreter::Value>> v_args);

//     /// @brief ACSII码转为BCD码
//     /// @param bcd BCD码字符串
//     /// @param asc ACSII码字符串
//     /// @param asc_len BCD码字符串的长度
//     void AscToBcd(unsigned char* bcd, unsigned char* asc, int asc_len);

//     /// @brief BCD码转为ACSII码;
//     /// @param asc ACSII码字符串
//     /// @param bcd BCD码字符串
//     /// @param bcd_len ACSII码字符串的长度
//     void BcdToAsc(char* asc, char* bcd, int bcd_len);

//     /// @brief ACSII码转为十六进制
//     /// @param hex HEX码字符串
//     /// @param asc ACSII码字符串
//     /// @param asc_len HEX码字符串的长度
//     void AscToHex(unsigned char* hex, unsigned char* asc, int asc_len);

//     /// @brief HEX码转为ACSII码;
//     /// @param asc ACSII码字符串
//     /// @param hex HEX码字符串
//     /// @param hex_len ACSII码字符串的长度
//     void HexToAsc(char* asc, char* hex, int hex_len);


//   private:
//     void GetModeType();

//     /// @brief 执行单DES算法对文本加密
//     /// @param type 加密类型：加密 ENCRYPT, 解密 DECRYPT
//     /// @param mode 加密模式：ECB, CBC
//     /// @param str_input 输入字符串
//     /// @param str_key 秘钥字符串
//     /// @param str_out 输出字符串
//     /// @param str_ivec CBC模式下，初始化向量
//     /// @return int [1]:加密成功 [0]:加密失败
//     /// @warning 以上字符串均为ASCII码
//     int Run1Des(int type,
//                 int mode,
//                 const char* str_input,
//                 const char* str_key,
//                 char* str_out,
//                 const char* str_ivec);

//     /// @brief
//     /// @param type
//     /// @param mode
//     /// @return
//     int Run1Des(int type,
//                 int mode,
//                 unsigned char* input,
//                 unsigned int in_len,
//                 unsigned char* key,
//                 unsigned int key_len,
//                 unsigned char* output,
//                 unsigned int out_len,
//                 unsigned char* ivec);

//     /// @brief 执行单DES算法对文本加密
//     /// @param type 加密类型：加密 ENCRYPT, 解密 DECRYPT
//     /// @param mode 加密模式：ECB, CBC
//     /// @param str_input 输入字符串
//     /// @param str_key 秘钥字符串
//     /// @param str_out 输出字符串
//     /// @param str_ivec CBC模式下，初始化向量
//     /// @return int [1]:加密成功 [0]:加密失败
//     /// @warning 以上字符串均为ASCII码
//     int Run3Des(int type,
//                 int mode,
//                 const char* str_input,
//                 const char* str_key,
//                 char* str_out,
//                 const char* str_ivec);

//     int Run3Des(int type,
//                 int mode,
//                 unsigned char* input,
//                 unsigned int in_len,
//                 unsigned char* key,
//                 unsigned int key_len,
//                 unsigned char* output,
//                 unsigned int out_len,
//                 unsigned char* ivec);

//     /// @brief 3Des 计算 Mac
//     /// @param str_input 输入字符串
//     /// @param str_key 秘钥字符串
//     /// @param out 输出字符串
//     /// @param str_ivec 初始化向量
//     /// @return int [1]:加密成功 [0]:加密失败
//     int PBOC_MAC(const char* str_input, const char* str_key, char* out, const char* str_ivec);

//     /// @brief 随机生成16进制字符串
//     /// @param hex 输出，生成的16进制随机字符串
//     /// @param len 输入，字符串长度
//     void RandomHex(char* hex, int len);

//     // 异或
//     void DataXOR(unsigned char* source, unsigned char* dest, int size, unsigned char* out);

//     /// @brief 将传入参数的ASCII码压缩为BCD码
//     /// @param str_input 输入字符串
//     /// @param str_key 秘钥字符串
//     /// @param str_out 输出字符串
//     /// @param str_ivec CEC模式下，初始化向量
//     void ArgsToBcd(const char* str_input, const char* str_key, char* str_out, const char* str_ivec);

//   private:
//     unsigned char input_[256];
//     int input_len_;

//     unsigned char key_[256];
//     int key_len_;

//     unsigned char ivec_[256];
//     int ivec_len_;

//     unsigned char output_[256];

//     int ret_; // 加密结果

//     const_DES_cblock block_key_[9];
//     DES_key_schedule key_schedule1_;
//     DES_key_schedule key_schedule2_;
//     DES_key_schedule key_schedule3_;
//     unsigned char out_[8];

//     // 定义Des加密模式的枚举
//     enum ModeType { DES_EDE_CBC, DES_EDE, DES_ECB, DES_CBC };
//     std::map<std::string, ModeType> m_mode_type_;
// };

// } // namespace function

// } // namespace script