#include <algorithm>
#include <memory>
#include <stdio.h>
#include <string.h>


namespace zel {

class String {
  public:
    String(const char* str = "") // 和传统方法相同
    {
        if (str == nullptr)
            str = "";

        str_ = new char[strlen(str) + 1];
        strcpy(str_, str);
    }
    // 拷贝构造函数
    String(const String& other) : str_(nullptr) {
        String temp(other.str_);    // 函数调用结束时会被析构
        std::swap(str_, temp.str_); // 交换
    }
    // 赋值运算符重载
    String& operator=(String s) // s对象此时为临时拷贝的一份资源，函数返回时会析构掉
    {
        if (this != &s) {
            std::swap(str_, s.str_); // 交换
        }
        return *this;
    }

    char* str() { return str_; }

    ~String() // 和传统方法相同
    {

        printf("析构函数：%s\n", str_);
        if (str_) {
            delete[] str_;
            str_ = nullptr;
        }
    }

  private:
    char* str_;
};
} // namespace zel

int main() {

    // zel::String string = "勇士总冠军";

    // zel::String aa = string;

    // printf("%s\n", aa.str());

    // 智能指针
    // 方法一
    std::shared_ptr<zel::String> str1(new zel::String("勇士"));

    auto str2 = std::make_shared<zel::String>("总冠军");

    auto str3 = str1;
    auto str4 = str1;

    printf("%s%s\n", str3->str(), str2->str());

    zel::String *str5 = new zel::String("是的");

    printf("%s\n", str5->str());

    return 0;
}