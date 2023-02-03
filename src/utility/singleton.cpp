#include "utility/singleton.h"

namespace zel {

namespace utility {

template <typename T>
Singleton<T>::Singleton() {}

template <typename T>
Singleton<T>::~Singleton() {}

template <typename T>
Singleton<T>::Singleton(const Singleton<T>&) {}

} // namespace utility

} // namespace zel