/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Contributors.
 * Created: Tue Jun 28 2022 01:06:52.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef F992FA91_47C1_45BA_A253_78C41F9B5D7F
#define F992FA91_47C1_45BA_A253_78C41F9B5D7F
#include <cstddef>
#include <type_traits>

namespace cxpr
{
template<typename T>
constexpr static inline const T log2(const T x)
{
    if(x < 2)
        return 0;
    return cxpr::log2<T>((x + 1) >> 1) + 1;
}

template<typename T, typename F>
constexpr static inline const T ceil(const F x)
{
    static_assert(std::is_integral_v<T>);
    static_assert(std::is_floating_point_v<F>);
    const T ival = static_cast<T>(x);
    if(ival == static_cast<F>(ival))
        return ival;
    return ival + ((x > 0) ? 1 : 0);
}

template<typename T>
constexpr static inline const T pow2(const T x)
{
    T value = 1;
    while(value < x)
        value *= 2;
    return value;
}

template<typename T>
constexpr static inline const T min(const T x, const T y)
{
    if(x > y)
        return y;
    return x;
}

template<typename T>
constexpr static inline const T max(const T x, const T y)
{
    if(x < y)
        return y;
    return x;
}

template<typename T>
constexpr static inline const T clamp(const T x, const T min, const T max)
{
    if(x < min)
        return min;
    if(x > max)
        return max;
    return x;
}

template<typename T, std::size_t L>
constexpr static inline const std::size_t arraySize(T (&)[L])
{
    return L;
}
} // namespace cxpr


#endif /* F992FA91_47C1_45BA_A253_78C41F9B5D7F */
