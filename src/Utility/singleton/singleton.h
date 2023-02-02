#pragma once

namespace zel {

namespace utility {

template <typename T>
class CSingleton {

  public:
    static T* Instance() {
        if (instance_ == nullptr)
            instance_ = new T();
        return instance_;
    }

  private:
    CSingleton();
    ~CSingleton();

    CSingleton(const CSingleton<T>&);

    CSingleton<T>& operator=(const CSingleton<T>);

  private:
    static T* instance_;
};

template <typename T>
T* CSingleton<T>::instance_ = nullptr;

} // namespace utility

} // namespace zel