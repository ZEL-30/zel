// #pragma once

// #include "../interpreter/value.h"

// namespace script {

// namespace function {

// class System {

//   public:
//     System(char* apdu, CReaderFactory* ReaderFac);

//     /// @brief 执行Apdu指令
//     /// @return CValue* 相应值
//     Value* Execute();

//     /// @brief 比对APDU返回值与期望值
//     /// @param v_expected 期望值容器
//     /// @return CValue*
//     Value* Compare(std::vector<CValue*> v_expected);

//     ~System();

//   private:
//     /// @brief Get the Apdu Type object
//     void GetApduType();

//     /// @brief Get the Str Expect object
//     void GetStrExpect();

//     void SendApdu();

//     void ResetCard();

//     bool CompareApdu();

//   private:
//     char apdu_[1024];                 // apdu指令
//     std::vector<CValue*> v_expected_; // 期望值容器
//     CReaderFactory* ReaderFac_;       // 读卡器

//     char response_[1024]; // 响应值字符串
//     char expected_[1024]; // 期望值字符串
//     bool compare_;        // 比对结果

//     // 定义指令类型的枚举
//     enum ApduType { APDU, RST, PTS };
//     std::map<std::string, ApduType> m_apdu_type_;
// };

// } // namespace function

// } // namespace script