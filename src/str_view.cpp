#include "str_view.h"
#include "str.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <vec.h>

// 构造函数实现
StrView::StrView(const StrView& lv) : _data(lv._data) {}
StrView::StrView(StrView&& rv) : _data(std::move(rv._data)) {}
StrView::StrView(const Str& lv) : _data(lv.std_str()) {}
StrView::StrView(Str&& rv) : _data(rv.std_str()) {}
StrView::StrView(const char* lv) : _data(lv) {}
StrView::StrView(const char* lv, size_t len) : _data(lv, len) {}
StrView::StrView(const char* lv, size_t off, size_t len) : _data(lv + off, len) {}
StrView::StrView(const std::string_view& lv) : _data(lv) {}
StrView::StrView(std::string_view&& rv) : _data(std::move(rv)) {}

size_t StrView::length() const {
    return _data.length();
}

bool StrView::is_empty() const {
    return _data.empty();
}

bool StrView::is_not_empty() const {
    return !_data.empty();
}

bool StrView::starts_with(const StrView& prefix) const {
    if (prefix.length() > _data.length())
        return false;
    return _data.substr(0, prefix.length()) == prefix._data;
}

bool StrView::ends_with(const StrView& suffix) const {
    if (suffix.length() > _data.length())
        return false;
    return _data.substr(_data.length() - suffix.length()) == suffix._data;
}

bool StrView::contains(const StrView& substr) const {
    return _data.find(substr._data) != std::string_view::npos;
}

StrView StrView::trim_start(const StrView& chars) const {
    size_t start = _data.find_first_not_of(chars._data);
    if (start == std::string_view::npos)
        return StrView("");
    return StrView(_data.data() + start, _data.length() - start);
}

StrView StrView::trim_end(const StrView& chars) const {
    size_t end = _data.find_last_not_of(chars._data);
    if (end == std::string_view::npos)
        return StrView("");
    return StrView(_data.data(), end + 1);
}

StrView StrView::trim(const StrView& chars) const {
    size_t start = _data.find_first_not_of(chars._data);
    if (start == std::string_view::npos)
        return StrView("");
    size_t end = _data.find_last_not_of(chars._data);
    return StrView(_data.data() + start, end - start + 1);
}

Vec<StrView> StrView::split(const std::string_view& delims) const {
    Vec<StrView> result;
    size_t prev = 0, pos = 0;
    while ((pos = _data.find_first_of(delims, prev)) != std::string_view::npos) {
        if (pos > prev)
            result.push_back(StrView(_data.data() + prev, pos - prev));
        prev = pos + 1;
    }
    if (prev < _data.length())
        result.push_back(StrView(_data.data() + prev, _data.length() - prev));
    return result;
}

StrView StrView::substr(size_t pos, size_t len) const {
    if (pos >= _data.length())
        return StrView("");
    return StrView(_data.data() + pos, std::min(len, _data.length() - pos));
}

StrView StrView::substr(size_t pos) const {
    if (pos >= _data.length())
        return StrView("");
    return StrView(_data.data() + pos, _data.length() - pos);
}

bool StrView::to_bool() const {
    std::string_view sv = _data;
    if (sv == "true" || sv == "1")
        return true;
    return false;
}

int StrView::to_int(int base) const {
    // 为了安全，创建一个临时的空终止字符串
    std::string temp(_data);
    return std::strtol(temp.c_str(), nullptr, base);
}

unsigned StrView::to_uint(int base) const {
    std::string temp(_data);
    return std::strtoul(temp.c_str(), nullptr, base);
}

int64_t StrView::to_int64(int base) const {
    std::string temp(_data);
    return std::strtoll(temp.c_str(), nullptr, base);
}

uint64_t StrView::to_uint64(int base) const {
    std::string temp(_data);
    return std::strtoull(temp.c_str(), nullptr, base);
}

float StrView::to_float() const {
    std::string temp(_data);
    return std::strtof(temp.c_str(), nullptr);
}

double StrView::to_double() const {
    std::string temp(_data);
    return std::strtod(temp.c_str(), nullptr);
}

void _test_str_view() {
//    // 基本构造和使用测试
//    StrView sv1("Hello");
//    const char* test = "Test String";
//    StrView sv2(test, 4);
//
//    // 功能测试
//    bool is_empty = sv1.is_empty();
//    bool starts = sv1.starts_with("He");
//    bool ends = sv1.ends_with("lo");
//    bool contains = sv1.contains("ell");
//
//    // 修剪测试
//    StrView padded = "  hello  ";
//    StrView trimmed = padded.trim();
//
//    // 分割测试
//    StrView csv = "a,b,c,d";
//    Vec<StrView> parts = csv.split(",");
//
//    // 转换测试
//    StrView num = "123";
//    int n = num.to_int();
//
//    // 子串测试
//    StrView sub = sv1.substr(1, 3); // "ell"
//
//    // 新增功能测试
//    // 查找功能
//    size_t pos = sv1.find('e');
//    size_t last = sv1.rfind('l');
//    size_t first_of = sv1.find_first_of("aeiou");
//    size_t last_not_of = sv1.find_last_not_of("ol");
//
//    // 修改视图
//    StrView sv3 = "Testing";
//    sv3.remove_prefix(2);  // "sting"
//    sv3.remove_suffix(1);  // "stin"
//
//    // 迭代器
//    for(auto it = sv1.begin(); it != sv1.end(); ++it) {
//        // 使用迭代器
//    }
//
//    // 元素访问
//    char first = sv1[0];
//    char last_char = sv1.back();
//
//    // 比较
//    bool less = sv1 < sv2;
//    int comp = sv1.compare(sv2);
}

