#ifndef __ENUM_H__
#define __ENUM_H__

#include <magic_enum.hpp>
#include <str.h>
#include <str_view.h>
#include <array.h>
#include <tuple.h>

template <typename Enum>
[[nodiscard]] Str enum_str(Enum value) {
    return Str(magic_enum::enum_name(value));
}

template <typename Enum>
[[nodiscard]] StrView enum_str_view(Enum value) {
    return StrView(magic_enum::enum_name(value));
}

template <typename Enum>
[[nodiscard]] Enum enum_cast(const char* str, Enum def_value) {
    auto opt = magic_enum::enum_cast<Enum>(str);
    if (opt.has_value()) {
        return opt.value();
    } else {
        return def_value;
    }
}

template <typename Enum>
[[nodiscard]] Enum enum_cast(const StrView& str_view, Enum def_value) {
    auto opt = magic_enum::enum_cast<Enum>(str_view.std_str_view());
    if (opt.has_value()) {
        return opt.value();
    } else {
        return def_value;
    }
}

template <typename Enum>
[[nodiscard]] Enum enum_cast(const Str& str, Enum def_value) {
    auto opt = magic_enum::enum_cast<Enum>(str.c_str());
    if (opt.has_value()) {
        return opt.value();
    } else {
        return def_value;
    }
}

template <typename Enum, size_t N = magic_enum::enum_count<Enum>()>
[[nodiscard]] Array<Enum, N> enum_values() {
    return std::array<Enum, N>(magic_enum::enum_values<Enum>());
}

template <typename Enum, size_t N = magic_enum::enum_count<Enum>()>
[[nodiscard]] Array<std::string_view,N> enum_names() {
    return Array<std::string_view,N>(magic_enum::enum_names<Enum>());
}

template <typename Enum, size_t N = magic_enum::enum_count<Enum>()>
[[nodiscard]] Array<std::pair<Enum,std::string_view>,N> enum_entries() {
    return Array<std::pair<Enum,std::string_view>,N>(magic_enum::enum_entries<Enum>());
}

using magic_enum::enum_count;

extern void _test_enum();

#endif
