#include "test.hpp"

#include <iostream>
#include <mutex>
#include <singleton.hpp>
#include <thread>
#include <unistd.h>

using namespace zel::utility;

std::mutex g_mutex; // 全局互斥锁

void Run(Test* test) {

    g_mutex.lock();
    std::cout << "线程: " << std::this_thread::get_id() << "\t";
    test->show();
    g_mutex.unlock();
}

int main() {

    auto test = zel::utility::Singleton<Test>::instance();

    test->age(24);
    test->name("ZEL");

    // 子线程
    for (int i = 0; i < 10; i++) {
        std::thread th(Run, test);

        th.detach();
    }

    sleep(1);

    return 0;
}