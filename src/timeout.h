/*
 *  Created on: Feb 28, 2024
 *      Author: houxd
 */

#ifndef TIMEOUT_H_
#define TIMEOUT_H_

#include <poll.h>
#include <types.h>

#ifdef _QT
#include <QDateTime>
inline uint32_t get_tick_ms() { return QDateTime::currentMSecsSinceEpoch(); }
#elif defined(_STM32)
#include <hal.h>
inline uint32_t get_tick_ms() { return HAL_GetTick(); }
#else
#include <sys/time.h>
#include <chrono>
inline uint32_t get_tick_ms() {
    uint32_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return ms;
}
#endif

struct Timeout : public Poll {
    using Poll::Poll;
    void stop() const;
};
using TimeoutFunc = Func<void()>;
using TimeoutFunc_1 = Func<void(Timeout timer)>;
extern Timeout set_timeout(uint32_t ms, const TimeoutFunc &cb);
extern Timeout set_timeout(uint32_t ms, const TimeoutFunc_1 &cb);
extern Timeout set_interval(uint32_t ms, const TimeoutFunc &cb);
extern Timeout set_interval(uint32_t ms, const TimeoutFunc_1 &cb);
extern void _test_timeout();

#endif /* TIMEOUT_H_ */
