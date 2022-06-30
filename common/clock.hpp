/* SPDX-License-Identifier: MPL-2.0 */
/*
 * Copyright (c), 2022, Voxelius Team.
 * Created: Tue Jun 28 2022 01:09:42.
 * Author: Kirill GPRB.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once
#include <chrono>

template<typename chronos>
class Clock final {
public:
    using duration = typename chronos::duration;
    using time_point = typename chronos::time_point;

public:
    Clock();
    const duration elapsed() const;
    const duration reset();

private:
    time_point start_time;

public:
    static const double seconds(const duration &d);
    static const time_point now();
};

template<typename chronos>
inline Clock<chronos>::Clock()
    : start_time(now())
{

}

template<typename chronos>
inline const typename Clock<chronos>::duration Clock<chronos>::elapsed() const
{
    return now() - start_time;
}

template<typename chronos>
inline const typename Clock<chronos>::duration Clock<chronos>::reset()
{
    const time_point new_start = now();
    const duration delta = new_start - start_time;
    start_time = new_start;
    return delta;
}

template<typename chronos>
inline const double Clock<chronos>::seconds(const Clock<chronos>::duration &d)
{
    return 1.0e-6 * static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(d).count());
}

template<typename chronos>
inline const typename Clock<chronos>::time_point Clock<chronos>::now()
{
    return chronos::now();
}
