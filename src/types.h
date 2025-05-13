#ifndef TYPES_H
#define TYPES_H

#include <memory>
#include <array>
#include <tuple>
#include <functional>
#include <magic_enum.hpp>
#include <str.h>
#include <str_view.h>
#include <vec.h>
#include <list>
#include <map>

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

template <typename T> using Func = std::function<T>;

template <typename T> using Unique = std::unique_ptr<T>;
template <typename T> using Shared = std::shared_ptr<T>;
template <typename T> using Weak = std::weak_ptr<T>;
using std::make_unique;
using std::make_shared;

template<typename Enum>
[[nodiscard]] const char* enum2str(Enum e) {
    static Str res;
    res = magic_enum::enum_name(e);
    return res.c_str();
}

template <typename Enum>
[[nodiscard]] Enum str2enum(const char* str, Enum def_value) {
    auto opt = magic_enum::enum_cast<Enum>(str);
    if (opt.has_value()) {
        return opt.value();
    } else {
        return def_value;
    }
}

template <typename Enum>
[[nodiscard]] constexpr size_t enum_count() {
    return magic_enum::enum_count<Enum>();
}

template <typename Enum>
[[nodiscard]] Vec<StrView> enum_keys() {
    Vec<StrView> keys;
    for (auto e : magic_enum::enum_values<Enum>()) {
        keys.push_back(magic_enum::enum_name(e));
    }
    return keys;
}

template <typename Enum>
[[nodiscard]] Vec<Enum> enum_values() {
    Vec<Enum> values;
    for (auto e : magic_enum::enum_values<Enum>()) {
        values.push_back(e);
    }
    return values;
}


// Tuple
template <typename... T> class Tuple {
private:
    std::tuple<T...> data;

public:
    // 构造函数
    Tuple() = default;

    // 拷贝/移动构造
    Tuple(const Tuple &) = default;
    Tuple(Tuple &&) = default;

    // 可变参数构造
    template<typename... Args, typename = std::enable_if_t<sizeof...(Args) == sizeof...(T)>>
    Tuple(Args&&... args) : data(std::forward<Args>(args)...) {}

    // 赋值操作符
    Tuple &operator=(const Tuple &) = default;
    Tuple &operator=(Tuple &&) = default;

    // 提供通用访问接口
    template <size_t I> auto &get() { return std::get<I>(data); }
    template <size_t I> const auto &get() const { return std::get<I>(data); }

    // 提供简易访问接口
    auto &_0() { return std::get<0>(data); }
    const auto &_0() const { return std::get<0>(data); }

    auto &_1() { return std::get<1>(data); }
    const auto &_1() const { return std::get<1>(data); }

    auto &_2() { return std::get<2>(data); }
    const auto &_2() const { return std::get<2>(data); }

    auto &_3() { return std::get<3>(data); }
    const auto &_3() const { return std::get<3>(data); }

    auto &_4() { return std::get<4>(data); }
    const auto &_4() const { return std::get<4>(data); }

    auto &_5() { return std::get<5>(data); }
    const auto &_5() const { return std::get<5>(data); }

    auto &_6() { return std::get<6>(data); }
    const auto &_6() const { return std::get<6>(data); }

    auto &_7() { return std::get<7>(data); }
    const auto &_7() const { return std::get<7>(data); }

    auto &_8() { return std::get<8>(data); }
    const auto &_8() const { return std::get<8>(data); }

    auto &_9() { return std::get<9>(data); }
    const auto &_9() const { return std::get<9>(data); }

    // 为结构化绑定提供访问
    const std::tuple<T...> &as_tuple() const & { return data; }
    std::tuple<T...> &as_tuple() & { return data; }
    std::tuple<T...> &&as_tuple() && { return std::move(data); }
};

namespace std {
template <typename... T> struct tuple_size<Tuple<T...>> : std::tuple_size<std::tuple<T...>> {};

template <size_t I, typename... T>
struct tuple_element<I, Tuple<T...>> : std::tuple_element<I, std::tuple<T...>> {};
} // namespace std

// 添加非成员get函数
template<size_t I, typename... T>
auto& tuple_get(Tuple<T...>& t) {
    return std::get<I>(t.as_tuple());
}

template<size_t I, typename... T>
const auto& tuple_get(const Tuple<T...>& t) {
    return std::get<I>(t.as_tuple());
}

template<size_t I, typename... T>
auto&& tuple_get(Tuple<T...>&& t) {
    return std::get<I>(std::move(t).as_tuple());
}

// List
template<typename T>
using List = std::list<T>;

// Map
template<typename K, typename V>
using Map = std::map<K,V>;

// Test
extern void _test_types();

#endif // TYPES_H
