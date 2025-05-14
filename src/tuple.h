#ifndef TUPLE_H
#define TUPLE_H

#include <tuple>
#include <cstdlib>

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

extern void test_tuple();

#endif
