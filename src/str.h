/*
 *  Copyright (C) 2020 by houxd
 *  All rights reserved.
 */
#ifndef STR_H
#define STR_H

#include <cstdint>
#include <printf.h>
#include <str_view.h>
#include <string>

class Str : public Printf {
    std::string _data;

  public:
    // 类型别名，与std::string一致
    using size_type = std::string::size_type;
    using iterator = std::string::iterator;
    using const_iterator = std::string::const_iterator;
    using reverse_iterator = std::string::reverse_iterator;
    using const_reverse_iterator = std::string::const_reverse_iterator;

    // 常量
    static const size_type npos = std::string::npos;

    // 构造函数
    Str() = default;
    Str(const Str &lv);
    Str(Str &&rv);
    Str(const StrView &lv);
    Str(StrView &&rv);
    Str(const char *lv);
    Str(const char *lv, size_t len);
    Str(const char *lv, size_t off, size_t len);
    Str(size_t n, char c);

    Str(const std::string &lv);
    Str(std::string &&rv);
    std::string &std_str() { return _data; }
    const std::string &std_str() const { return _data; }

    const char *c_str() const;

    size_t length() const;
    bool is_empty() const;
    bool is_not_empty() const;

    bool starts_with(const StrView &prefix) const;
    bool ends_with(const StrView &suffix) const;
    bool contains(const StrView &substr) const;

    StrView trim_start(const StrView &chars = "\t\n\v\f\r ") const;
    StrView trim_end(const StrView &chars = "\t\n\v\f\r ") const;
    StrView trim(const StrView &chars = "\t\n\v\f\r ") const;

    Str to_lower() const;
    Str to_upper() const;

    Str replace(const std::string_view &from, const std::string_view &to) const;
    Str replace_all(const std::string_view &from, const std::string_view &to) const;

    Vec<StrView> split(const std::string_view &delims) const;

    StrView substr(size_t pos, size_t len) const;
    StrView substr(size_t pos) const;

    bool to_bool() const;
    int to_int(int base = 10) const;
    unsigned to_uint(int base = 10) const;
    int64_t to_int64(int base = 10) const;
    uint64_t to_uint64(int base = 10) const;
    float to_float() const;
    double to_double() const;

    static Str from_bool(const bool &v);
    static Str from_int(const int &v, int base = 10);
    static Str from_uint(const unsigned &v, int base = 10);
    static Str from_int64(const int64_t &v, int base = 10);
    static Str from_uint16(const uint64_t &v, int base = 10);
    static Str from_float(const float &v);
    static Str from_double(const double &v);

    Str &append(const char c);
    Str &append(const char *s);
    Str &append(const char *s, size_t len);
    Str &append(const char *s, size_t off, size_t len);
    Str &append(const Str &v);
    Str &append(const StrView &v);

    Str& operator=(const Str &lv);
    Str& operator=(Str &&rv);
    Str& operator=(const StrView &lv);
    Str& operator=(StrView &&rv);
    Str& operator+(const Str &lv);
    Str& operator+(Str &&lv);
    Str& operator+=(const Str &lv);
    Str& operator+=(Str &&lv);
    bool operator==(const Str &lv) const;
    bool operator!=(const Str &lv) const;

    // 迭代器相关
    iterator begin() { return _data.begin(); }
    const_iterator begin() const { return _data.begin(); }
    const_iterator cbegin() const { return _data.cbegin(); }
    iterator end() { return _data.end(); }
    const_iterator end() const { return _data.end(); }
    const_iterator cend() const { return _data.cend(); }
    reverse_iterator rbegin() { return _data.rbegin(); }
    const_reverse_iterator rbegin() const { return _data.rbegin(); }
    const_reverse_iterator crbegin() const { return _data.crbegin(); }
    reverse_iterator rend() { return _data.rend(); }
    const_reverse_iterator rend() const { return _data.rend(); }
    const_reverse_iterator crend() const { return _data.crend(); }

    // 容量相关
    size_type size() const { return _data.size(); }
    size_type max_size() const { return _data.max_size(); }
    void resize(size_type n) { _data.resize(n); }
    void resize(size_type n, char c) { _data.resize(n, c); }
    size_type capacity() const { return _data.capacity(); }
    void reserve(size_type n = 0) { _data.reserve(n); }
    void shrink_to_fit() { _data.shrink_to_fit(); }
    void clear() { _data.clear(); }

    // 元素访问
    char& operator[](size_type pos) { return _data[pos]; }
    const char& operator[](size_type pos) const { return _data[pos]; }
    char& at(size_type pos) { return _data.at(pos); }
    const char& at(size_type pos) const { return _data.at(pos); }
    char& front() { return _data.front(); }
    const char& front() const { return _data.front(); }
    char& back() { return _data.back(); }
    const char& back() const { return _data.back(); }

    // 修改操作
    Str& insert(size_type pos, const Str& str) {
        _data.insert(pos, str._data);
        return *this;
    }
    Str& insert(size_type pos, const char* s) {
        _data.insert(pos, s);
        return *this;
    }
    Str& insert(size_type pos, const char* s, size_type n) {
        _data.insert(pos, s, n);
        return *this;
    }
    Str& insert(size_type pos, size_type n, char c) {
        _data.insert(pos, n, c);
        return *this;
    }

    Str& erase(size_type pos = 0, size_type n = npos) {
        _data.erase(pos, n);
        return *this;
    }
    
    void push_back(char c) { _data.push_back(c); }
    void pop_back() { _data.pop_back(); }
    
    Str& assign(const Str& str) {
        _data.assign(str._data);
        return *this;
    }
    Str& assign(const char* s) {
        _data.assign(s);
        return *this;
    }
    Str& assign(const char* s, size_type n) {
        _data.assign(s, n);
        return *this;
    }

    // 查找相关
    size_type find(const Str& str, size_type pos = 0) const {
        return _data.find(str._data, pos);
    }
    size_type find(const char* s, size_type pos = 0) const {
        return _data.find(s, pos);
    }
    size_type find(const char* s, size_type pos, size_type n) const {
        return _data.find(s, pos, n);
    }
    size_type find(char c, size_type pos = 0) const {
        return _data.find(c, pos);
    }

    size_type rfind(const Str& str, size_type pos = npos) const {
        return _data.rfind(str._data, pos);
    }
    size_type rfind(const char* s, size_type pos = npos) const {
        return _data.rfind(s, pos);
    }
    size_type rfind(const char* s, size_type pos, size_type n) const {
        return _data.rfind(s, pos, n);
    }
    size_type rfind(char c, size_type pos = npos) const {
        return _data.rfind(c, pos);
    }

    size_type find_first_of(const Str& str, size_type pos = 0) const {
        return _data.find_first_of(str._data, pos);
    }
    size_type find_first_of(const char* s, size_type pos = 0) const {
        return _data.find_first_of(s, pos);
    }
    size_type find_first_of(const char* s, size_type pos, size_type n) const {
        return _data.find_first_of(s, pos, n);
    }
    size_type find_first_of(char c, size_type pos = 0) const {
        return _data.find_first_of(c, pos);
    }

    size_type find_last_of(const Str& str, size_type pos = npos) const {
        return _data.find_last_of(str._data, pos);
    }
    size_type find_last_of(const char* s, size_type pos = npos) const {
        return _data.find_last_of(s, pos);
    }
    size_type find_last_of(const char* s, size_type pos, size_type n) const {
        return _data.find_last_of(s, pos, n);
    }
    size_type find_last_of(char c, size_type pos = npos) const {
        return _data.find_last_of(c, pos);
    }

    size_type find_first_not_of(const Str& str, size_type pos = 0) const {
        return _data.find_first_not_of(str._data, pos);
    }
    size_type find_first_not_of(const char* s, size_type pos = 0) const {
        return _data.find_first_not_of(s, pos);
    }
    size_type find_first_not_of(const char* s, size_type pos, size_type n) const {
        return _data.find_first_not_of(s, pos, n);
    }
    size_type find_first_not_of(char c, size_type pos = 0) const {
        return _data.find_first_not_of(c, pos);
    }

    size_type find_last_not_of(const Str& str, size_type pos = npos) const {
        return _data.find_last_not_of(str._data, pos);
    }
    size_type find_last_not_of(const char* s, size_type pos = npos) const {
        return _data.find_last_not_of(s, pos);
    }
    size_type find_last_not_of(const char* s, size_type pos, size_type n) const {
        return _data.find_last_not_of(s, pos, n);
    }
    size_type find_last_not_of(char c, size_type pos = npos) const {
        return _data.find_last_not_of(c, pos);
    }

    // 比较
    int compare(const Str& str) const {
        return _data.compare(str._data);
    }
    int compare(const char* s) const {
        return _data.compare(s);
    }
    int compare(size_type pos, size_type n, const Str& str) const {
        return _data.compare(pos, n, str._data);
    }
    int compare(size_type pos, size_type n, const char* s) const {
        return _data.compare(pos, n, s);
    }
    int compare(size_type pos, size_type n, const Str& str, size_type pos2, size_type n2) const {
        return _data.compare(pos, n, str._data, pos2, n2);
    }
    int compare(size_type pos, size_type n, const char* s, size_type n2) const {
        return _data.compare(pos, n, s, n2);
    }

    // 补充运算符
    Str operator+(const Str& rhs) const {
        Str result(*this);
        result._data += rhs._data;
        return result;
    }
    Str operator+(const char* rhs) const {
        Str result(*this);
        result._data += rhs;
        return result;
    }
    Str operator+(char rhs) const {
        Str result(*this);
        result._data += rhs;
        return result;
    }
    
    Str& operator+=(const char* s) {
        _data += s;
        return *this;
    }
    Str& operator+=(char c) {
        _data += c;
        return *this;
    }
    
    // 比较运算符
    bool operator<(const Str& rhs) const { return _data < rhs._data; }
    bool operator>(const Str& rhs) const { return _data > rhs._data; }
    bool operator<=(const Str& rhs) const { return _data <= rhs._data; }
    bool operator>=(const Str& rhs) const { return _data >= rhs._data; }

    static Str sprintf(const char* fmt, ...) {
        Str result;
        va_list args;
        va_start(args, fmt);
        result.vprintf(fmt, args);
        va_end(args);
        return result;
    }

  private:
    // Implement Printf Interface
    void printf_write_char(char c) override;
};

// 全局运算符
inline Str operator+(const char* lhs, const Str& rhs) {
    Str result(lhs);
    result += rhs;
    return result;
}

inline Str operator+(char lhs, const Str& rhs) {
    Str result(1, lhs);
    result += rhs;
    return result;
}

extern void _test_str();

#endif // STR_H
