#ifndef __IO_FACTORY_FACTORY__
#define __IO_FACTORY_FACTORY__

#include <define.h>

#include <functional>

template <typename T>
using FFactory = std::function<T*()>;

#endif