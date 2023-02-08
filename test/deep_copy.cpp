#include <string>
#include<algorithm>


namespace zel {

class string {
  public:
    string(const char* str = "") // 和传统方法相同
    {
        if (str == nullptr)
            str = "";

        str_ = new char[strlen(str) + 1];
        strcpy(str_, str);
    }
    // 拷贝构造函数
    string(const string& other) : str_(nullptr) {
        string temp(other.str_);   // 函数调用结束时会被析构
        std::swap(str_, temp.str_); // 交换
    }
    // 赋值运算符重载
    string& operator=(string s) // s对象此时为临时拷贝的一份资源，函数返回时会析构掉
    {
        if (this != &s) {
            std::swap(str_, s.str_); // 交换
        }
        return *this;
    }
    ~string() // 和传统方法相同
    {
        if (str_) {
            delete[] str_;
            str_ = nullptr;
        }
    }

  private:
    char* str_;
};
} // namespace bit2


int main() {

    zel::string string = "勇士总冠军";

    zel::string aa = string;

    printf("%s\n", aa);

    return 0;
}