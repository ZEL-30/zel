#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <utility/singleton.hpp>


using namespace zel::utility;

std::mutex g_mutex; // 全局互斥锁

class A {

    friend class zel::utility::Singleton<A>;

  public:
    void show() { std::cout << name_ << std::endl; }

  private:
    A() : name_("A") {}
    ~A() {}
    A(const A&) {}

    A& operator=(const A&);

  private:
    std::string name_;
};

void Run(A* a) {

    g_mutex.lock();
    std::cout << "线程: " << std::this_thread::get_id() << "\t";
    a->show();
    g_mutex.unlock();
}

int main() {

    // 主线程
    A* a = Singleton<A>::instance();

    // 子线程
    for (int i = 0; i < 10; i++) {
        std::thread th(Run, a);

        th.detach();
    }

    sleep(1);

    return 0;
}