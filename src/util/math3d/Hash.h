#ifndef __UTIL_MATH3D_HASH__
#define __UTIL_MATH3D_HASH__

#include <define.h>

namespace simple3deditor {

inline size_t Hash(const byte* s, size_t len) noexcept {
    size_t hash = len;
    for (size_t i = 0; i < len; i++)
        hash = 31 * hash + (size_t)s[i];
    return hash;
}

template <typename T>
inline size_t Hash(const T& v){
    return Hash(static_cast<byte*>(&v), sizeof(T));
}

template <typename T>
inline void HashCombine(size_t& seed, const T& v) {
    seed ^= Hash(v) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
}

}

#endif