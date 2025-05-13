#include "str.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>

// 构造函数实现
Str::Str(const Str &lv) : _data(lv._data) {}
Str::Str(Str &&rv) : _data(std::move(rv._data)) {}
Str::Str(const StrView &lv) : _data(lv.std_str_view()) {}
Str::Str(StrView &&rv) : _data(rv.std_str_view()) {}
Str::Str(const char *lv) : _data(lv) {}
Str::Str(const char *lv, size_t len) : _data(lv, len) {}
Str::Str(const char *lv, size_t off, size_t len) : _data(lv + off, len) {}

Str::Str(size_t n, char c): _data(n, c) {}
Str::Str(const std::string &lv) : _data(lv) {}
Str::Str(std::string &&rv) : _data(std::move(rv)) {}

const char *Str::c_str() const {
    return _data.c_str();
}

size_t Str::length() const {
    return _data.length();
}

bool Str::is_empty() const {
    return _data.empty();
}

bool Str::is_not_empty() const {
    return !_data.empty();
}

bool Str::starts_with(const StrView &prefix) const {
    return _data.compare(0, prefix.length(), prefix.std_str_view()) == 0;
}

bool Str::ends_with(const StrView &suffix) const {
    if (suffix.length() > _data.length())
        return false;
    return _data.compare(_data.length() - suffix.length(), 
                        suffix.length(), 
                        suffix.std_str_view()) == 0;
}

bool Str::contains(const StrView &substr) const {
    return _data.find(substr.std_str_view()) != std::string::npos;
}

StrView Str::trim_start(const StrView &chars) const {
    size_t start = _data.find_first_not_of(chars.std_str_view());
    if (start == std::string::npos)
        return StrView("");
    return StrView(_data.c_str() + start, _data.length() - start);
}

StrView Str::trim_end(const StrView &chars) const {
    size_t end = _data.find_last_not_of(chars.std_str_view());
    if (end == std::string::npos)
        return StrView("");
    return StrView(_data.c_str(), end + 1);
}

StrView Str::trim(const StrView &chars) const {
    size_t start = _data.find_first_not_of(chars.std_str_view());
    if (start == std::string::npos)
        return StrView("");
    size_t end = _data.find_last_not_of(chars.std_str_view());
    return StrView(_data.c_str() + start, end - start + 1);
}

Str Str::to_lower() const {
    std::string result = _data;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return Str(std::move(result));
}

Str Str::to_upper() const {
    std::string result = _data;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return Str(std::move(result));
}

Str Str::replace(const std::string_view &from, const std::string_view &to) const {
    std::string result = _data;
    size_t pos = result.find(from);
    if (pos != std::string::npos)
        result.replace(pos, from.length(), to);
    return Str(std::move(result));
}

Str Str::replace_all(const std::string_view &from, const std::string_view &to) const {
    if (from.empty())
        return *this;

    std::string result = _data;
    size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    return Str(std::move(result));
}

Vec<StrView> Str::split(const std::string_view &delims) const {
    Vec<StrView> result;
    size_t prev = 0, pos = 0;
    while ((pos = _data.find_first_of(delims, prev)) != std::string::npos) {
        if (pos > prev)
            result.push_back(StrView(_data.c_str() + prev, pos - prev));
        prev = pos + 1;
    }
    if (prev < _data.length())
        result.push_back(StrView(_data.c_str() + prev, _data.length() - prev));
    return result;
}

StrView Str::substr(size_t pos, size_t len) const {
    if (pos >= _data.length())
        return StrView("");
    return StrView(_data.c_str() + pos, std::min(len, _data.length() - pos));
}

StrView Str::substr(size_t pos) const {
    if (pos >= _data.length())
        return StrView("");
    return StrView(_data.c_str() + pos, _data.length() - pos);
}

bool Str::to_bool() const {
    if (_data == "true" || _data == "1")
        return true;
    return false;
}

int Str::to_int(int base) const {
    return std::strtol(_data.c_str(), nullptr, base);
}

unsigned Str::to_uint(int base) const {
    return std::strtoul(_data.c_str(), nullptr, base);
}

int64_t Str::to_int64(int base) const {
    return std::strtoll(_data.c_str(), nullptr, base);
}

uint64_t Str::to_uint64(int base) const {
    return std::strtoull(_data.c_str(), nullptr, base);
}

float Str::to_float() const {
    return std::strtof(_data.c_str(), nullptr);
}

double Str::to_double() const {
    return std::strtod(_data.c_str(), nullptr);
}

Str Str::from_bool(const bool &v) {
    return v ? Str("true") : Str("false");
}

Str Str::from_int(const int &v, int base) {
    char buf[32];
    if (base == 10)
        snprintf(buf, sizeof(buf), "%d", v);
    else if (base == 16)
        snprintf(buf, sizeof(buf), "%x", v);
    else if (base == 8)
        snprintf(buf, sizeof(buf), "%o", v);
    else
        snprintf(buf, sizeof(buf), "%d", v);
    return Str(buf);
}

Str Str::from_uint(const unsigned &v, int base) {
    char buf[32];
    if (base == 10)
        snprintf(buf, sizeof(buf), "%u", v);
    else if (base == 16)
        snprintf(buf, sizeof(buf), "%x", v);
    else if (base == 8)
        snprintf(buf, sizeof(buf), "%o", v);
    else
        snprintf(buf, sizeof(buf), "%u", v);
    return Str(buf);
}

Str Str::from_int64(const int64_t &v, int base) {
    char buf[64];
    if (base == 10)
        snprintf(buf, sizeof(buf), "%lld", (long long)v);
    else if (base == 16)
        snprintf(buf, sizeof(buf), "%llx", (long long)v);
    else if (base == 8)
        snprintf(buf, sizeof(buf), "%llo", (long long)v);
    else
        snprintf(buf, sizeof(buf), "%lld", (long long)v);
    return Str(buf);
}

Str Str::from_uint16(const uint64_t &v, int base) {
    char buf[64];
    if (base == 10)
        snprintf(buf, sizeof(buf), "%llu", (unsigned long long)v);
    else if (base == 16)
        snprintf(buf, sizeof(buf), "%llx", (unsigned long long)v);
    else if (base == 8)
        snprintf(buf, sizeof(buf), "%llo", (unsigned long long)v);
    else
        snprintf(buf, sizeof(buf), "%llu", (unsigned long long)v);
    return Str(buf);
}

Str Str::from_float(const float &v) {
	return Str::sprintf("%f", v);
}

Str Str::from_double(const double &v) {
	return Str::sprintf("%lf", v);
}

// 附加操作
Str &Str::append(const char c) {
    _data.push_back(c);
    return *this;
}

Str &Str::append(const char *s) {
    _data.append(s);
    return *this;
}

Str &Str::append(const char *s, size_t len) {
    _data.append(s, len);
    return *this;
}

Str &Str::append(const char *s, size_t off, size_t len) {
    _data.append(s + off, len);
    return *this;
}

Str &Str::append(const Str &v) {
    _data.append(v._data);
    return *this;
}

Str &Str::append(const StrView &v) {
    _data.append(v.std_str_view());
    return *this;
}

// 运算符实现
Str& Str::operator=(const Str &lv) {
    _data = lv._data;
    return *this;
}

Str& Str::operator=(Str &&rv) {
    _data = std::move(rv._data);
    return *this;
}

Str& Str::operator=(const StrView &lv) {
    _data = lv.std_str_view();
    return *this;
}

Str& Str::operator=(StrView &&rv) {
    _data = rv.std_str_view();
    return *this;
}

Str& Str::operator+(const Str &lv) {
    Str result = *this;
    result._data += lv._data;
    return *new Str(std::move(result));
}

Str& Str::operator+(Str &&lv) {
    Str result = *this;
    result._data += lv._data;
    return *new Str(std::move(result));
}

Str& Str::operator+=(const Str &lv) {
    _data += lv._data;
    return *this;
}

Str& Str::operator+=(Str &&lv) {
    _data += lv._data;
    return *this;
}

bool Str::operator==(const Str &lv) const {
    return _data == lv._data;
}

bool Str::operator!=(const Str &lv) const {
    return _data != lv._data;
}

// Printf接口实现
void Str::printf_write_char(char c) {
    _data.push_back(c);
}

void _test_str() {
//    // 基本构造和转换测试
//    Str s1("Hello");
//    Str s2(" World");
//    Str s3 = s1 + s2;
//
//    // 功能测试
//    bool is_empty = s1.is_empty();
//    bool starts = s1.starts_with("He");
//    bool ends = s1.ends_with("lo");
//    bool contains = s3.contains("o W");
//
//    // 转换测试
//    Str upper = s1.to_upper();
//    Str lower = upper.to_lower();
//
//    // 数值转换测试
//    Str num("123");
//    int n = num.to_int();
//    Str num_str = Str::from_int(n);
//
//    // 修改测试
//    Str replaced = s3.replace("World", "Friend");
//
//    // 分割测试
//    Str csv("a,b,c,d");
//    Vec<StrView> parts = csv.split(",");
//
//    // 新增功能测试
//    // 查找功能
//    size_t pos = s3.find("World");
//    size_t last = s3.rfind('l');
//    size_t first_of = s3.find_first_of("aeiou");
//    size_t last_not_of = s3.find_last_not_of(" ");
//
//    // 插入和删除
//    Str s4 = "Test";
//    s4.insert(2, "XX");
//    s4.erase(1, 2);
//
//    // 迭代器
//    for(auto it = s1.begin(); it != s1.end(); ++it) {
//        // 使用迭代器
//    }
//
//    // 元素访问
//    char first = s1[0];
//    char last_char = s1.back();
//
//    // 比较
//    bool less = s1 < s2;
//    int comp = s1.compare(s2);
//
//    // 容量操作
//    s1.reserve(100);
//    size_t cap = s1.capacity();
//    s1.shrink_to_fit();
}
