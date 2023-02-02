#include "singleton.h"

namespace zel {

namespace utility {

template <typename T>
CSingleton<T>::CSingleton() {}

template <typename T>
CSingleton<T>::~CSingleton() {}

template <typename T>
CSingleton<T>::CSingleton(const CSingleton<T>&) {}

} // namespace utility

} // namespace zel