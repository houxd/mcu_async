/*
 *  Created on: Feb 28, 2024
 *      Author: houxd
 */

#include <memory>
#include <poll.h>
#include <timeout.h>

void Timeout::stop() const {
    return this->remove();
}

Timeout set_timeout(uint32_t ms, const TimeoutFunc &cb) {
    uint32_t escape = get_tick_ms() + ms;
    Poll poll = set_poll([=](Poll poll) {
        if (get_tick_ms() >= escape) {
            cb();
            poll.remove();
        }
    });
    return {poll.id};
}

Timeout set_timeout(uint32_t ms, const TimeoutFunc_1 &cb) {
    uint32_t escape = get_tick_ms() + ms;
    Poll pid = set_poll([=](Poll pid) {
        if (get_tick_ms() >= escape) {
            cb({pid.id});
            pid.remove();
        }
    });
    return {pid.id};
}

Timeout set_interval(uint32_t ms, const TimeoutFunc &cb) {
    auto escape = std::make_shared<uint32_t>(get_tick_ms() + ms);
    Poll pid = set_poll([=](Poll) {
        uint32_t now = get_tick_ms();
        if (now >= *escape) {
            cb();
            *escape = now + ms;
        }
    });
    return {pid.id};
}

Timeout set_interval(uint32_t ms, const TimeoutFunc_1 &cb) {
    auto escape = std::make_shared<uint32_t>(get_tick_ms() + ms);
    Poll pid = set_poll([=](Poll pid) {
        uint32_t now = get_tick_ms();
        if (now >= *escape) {
            cb({pid.id});
            *escape = now + ms;
        }
    });
    return {pid.id};
}

void _test_timeout() {
    // 测试 set_timeout
    set_timeout(1000, [] {
        printf("Timeout 1s\n");
    });
    // 测试 set_interval
    set_interval(500, [] {
        printf("Interval 500ms\n");
    });
}
