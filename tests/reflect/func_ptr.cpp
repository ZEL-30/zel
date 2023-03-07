#include <iostream>

using namespace std;

typedef int (*func)(int, int);

int substraction(int a, int b) { return a / b; }

class Math {

  public:
    template <typename T>
    T addition(T a, T b) {
        return a + b;
    }
};

typedef std::function<int(Math*(int, int))> MathMethod;

void funcPtr(func f, int a, int b) {

    int c = f(a, b);

    cout << c << endl;
}

int main() {

    // 全局函数
    func f = &substraction;
    funcPtr(f, 8, 4);

    // 类成员函数


    return 0;
}