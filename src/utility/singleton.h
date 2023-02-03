#pragma once

namespace zel {

namespace utility {

template <typename T>
class Singleton {

  public:
    static T* Instance() {
        if (instance_ == nullptr)
            instance_ = new T();
        return instance_;
    }

  private:
    Singleton();
    ~Singleton();

    Singleton(const Singleton<T>&);

    Singleton<T>& operator=(const Singleton<T>);

  private:
    static T* instance_;
};

template <typename T>
T* Singleton<T>::instance_ = nullptr;

} // namespace utility

} // namespace zel