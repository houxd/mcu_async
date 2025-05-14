#ifndef TYPES_H
#define TYPES_H

#include <memory>
#include <functional>

#ifdef QT_CORE_LIB
#define _QT
#endif

#if defined(__arm__) && defined(USE_HAL_DRIVER)
#define _STM32
#endif

#undef uint
typedef unsigned uint;

#undef u8
#undef u16
#undef u32
#undef u64
#undef s8
#undef s16
#undef s32
#undef s64
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define s8 int8_t
#define s16 int16_t
#define s32 int32_t
#define s64 int64_t

#include <tuple.h>
#include <list.h>
#include <map.h>
#include <set.h>
#include <vec.h>
#include <enum.h>

template <typename T> using Func = std::function<T>;

template <typename T> using Unique = std::unique_ptr<T>;
template <typename T> using Shared = std::shared_ptr<T>;
template <typename T> using Weak = std::weak_ptr<T>;

using std::make_unique;
using std::make_shared;

// Test
extern void _test_types();

#endif // TYPES_H
