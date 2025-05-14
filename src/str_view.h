#ifndef STR_VIEW_H
#define STR_VIEW_H

#include <cstdint>
#include <string_view>

template <typename T> class Vec;

class Str;

class StrView {
    std::string_view _data;
public:
    // 类型别名，与std::string_view一致
    using size_type = std::string_view::size_type;
    using const_iterator = std::string_view::const_iterator;
    using const_reverse_iterator = std::string_view::const_reverse_iterator;
    
    // 常量
    static const size_type npos = std::string_view::npos;
    
    // 默认构造函数
    StrView() = default;

    StrView(const StrView& lv);
    StrView(StrView&& rv);
    StrView(const Str& lv);
    StrView(Str&& rv);
    StrView(const char* lv);
    StrView(const char* lv, size_t len);
    StrView(const char* lv, size_t off, size_t len);

    StrView(const std::string_view& lv);
    StrView(std::string_view&& rv);
    std::string_view& std_str_view() { return _data; }
    const std::string_view& std_str_view() const { return _data; }

    // 迭代器相关
    const_iterator begin() const { return _data.begin(); }
    const_iterator cbegin() const { return _data.cbegin(); }
    const_iterator end() const { return _data.end(); }
    const_iterator cend() const { return _data.cend(); }
    const_reverse_iterator rbegin() const { return _data.rbegin(); }
    const_reverse_iterator crbegin() const { return _data.crbegin(); }
    const_reverse_iterator rend() const { return _data.rend(); }
    const_reverse_iterator crend() const { return _data.crend(); }
    
    // 容量相关
    size_type size() const { return _data.size(); }
    size_type max_size() const { return _data.max_size(); }
    bool empty() const { return _data.empty(); }
    
    size_t length() const;
    bool is_empty() const;
    bool is_not_empty() const;

    // 元素访问
    const char& operator[](size_type pos) const { return _data[pos]; }
    const char& at(size_type pos) const { return _data.at(pos); }
    const char& front() const { return _data.front(); }
    const char& back() const { return _data.back(); }
    const char* data() const { return _data.data(); }
    
    // 修改操作
    void remove_prefix(size_type n) { _data.remove_prefix(n); }
    void remove_suffix(size_type n) { _data.remove_suffix(n); }
    void swap(StrView& sv) { _data.swap(sv._data); }
    
    // 查找相关
    size_type find(const StrView& sv, size_type pos = 0) const {
        return _data.find(sv._data, pos);
    }
    size_type find(char c, size_type pos = 0) const {
        return _data.find(c, pos);
    }
    size_type find(const char* s, size_type pos = 0) const {
        return _data.find(s, pos);
    }
    size_type find(const char* s, size_type pos, size_type n) const {
        return _data.find(s, pos, n);
    }
    
    size_type rfind(const StrView& sv, size_type pos = npos) const {
        return _data.rfind(sv._data, pos);
    }
    size_type rfind(char c, size_type pos = npos) const {
        return _data.rfind(c, pos);
    }
    size_type rfind(const char* s, size_type pos = npos) const {
        return _data.rfind(s, pos);
    }
    size_type rfind(const char* s, size_type pos, size_type n) const {
        return _data.rfind(s, pos, n);
    }
    
    size_type find_first_of(const StrView& sv, size_type pos = 0) const {
        return _data.find_first_of(sv._data, pos);
    }
    size_type find_first_of(char c, size_type pos = 0) const {
        return _data.find_first_of(c, pos);
    }
    size_type find_first_of(const char* s, size_type pos = 0) const {
        return _data.find_first_of(s, pos);
    }
    size_type find_first_of(const char* s, size_type pos, size_type n) const {
        return _data.find_first_of(s, pos, n);
    }
    
    size_type find_last_of(const StrView& sv, size_type pos = npos) const {
        return _data.find_last_of(sv._data, pos);
    }
    size_type find_last_of(char c, size_type pos = npos) const {
        return _data.find_last_of(c, pos);
    }
    size_type find_last_of(const char* s, size_type pos = npos) const {
        return _data.find_last_of(s, pos);
    }
    size_type find_last_of(const char* s, size_type pos, size_type n) const {
        return _data.find_last_of(s, pos, n);
    }
    
    size_type find_first_not_of(const StrView& sv, size_type pos = 0) const {
        return _data.find_first_not_of(sv._data, pos);
    }
    size_type find_first_not_of(char c, size_type pos = 0) const {
        return _data.find_first_not_of(c, pos);
    }
    size_type find_first_not_of(const char* s, size_type pos = 0) const {
        return _data.find_first_not_of(s, pos);
    }
    size_type find_first_not_of(const char* s, size_type pos, size_type n) const {
        return _data.find_first_not_of(s, pos, n);
    }
    
    size_type find_last_not_of(const StrView& sv, size_type pos = npos) const {
        return _data.find_last_not_of(sv._data, pos);
    }
    size_type find_last_not_of(char c, size_type pos = npos) const {
        return _data.find_last_not_of(c, pos);
    }
    size_type find_last_not_of(const char* s, size_type pos = npos) const {
        return _data.find_last_not_of(s, pos);
    }
    size_type find_last_not_of(const char* s, size_type pos, size_type n) const {
        return _data.find_last_not_of(s, pos, n);
    }
    
    bool starts_with(const StrView& prefix) const;
    bool ends_with(const StrView& suffix) const;
    bool contains(const StrView& substr) const;

    StrView trim_start(const StrView& chars = "\t\n\v\f\r ") const;
    StrView trim_end(const StrView& chars = "\t\n\v\f\r ") const;
    StrView trim(const StrView& chars = "\t\n\v\f\r ") const;

    Vec<StrView> split(const std::string_view& delims) const;

    StrView substr(size_t pos, size_t len) const;
    StrView substr(size_t pos) const;

    bool to_bool() const;
    int to_int(int base = 10) const;
    unsigned to_uint(int base = 10) const;
    int64_t to_int64(int base = 10) const;
    uint64_t to_uint64(int base = 10) const;
    float to_float() const;
    double to_double() const;

    // 比较
    int compare(const StrView& sv) const {
        return _data.compare(sv._data);
    }
    int compare(size_type pos1, size_type n1, const StrView& sv) const {
        return _data.compare(pos1, n1, sv._data);
    }
    int compare(size_type pos1, size_type n1, const StrView& sv, size_type pos2, size_type n2) const {
        return _data.compare(pos1, n1, sv._data, pos2, n2);
    }
    int compare(const char* s) const {
        return _data.compare(s);
    }
    int compare(size_type pos1, size_type n1, const char* s) const {
        return _data.compare(pos1, n1, s);
    }
    int compare(size_type pos1, size_type n1, const char* s, size_type n2) const {
        return _data.compare(pos1, n1, s, n2);
    }

    // 运算符重载
    bool operator==(const StrView& rhs) const { return _data == rhs._data; }
    bool operator!=(const StrView& rhs) const { return _data != rhs._data; }
    bool operator<(const StrView& rhs) const { return _data < rhs._data; }
    bool operator>(const StrView& rhs) const { return _data > rhs._data; }
    bool operator<=(const StrView& rhs) const { return _data <= rhs._data; }
    bool operator>=(const StrView& rhs) const { return _data >= rhs._data; }
};

extern void _test_str_view();

#endif // STR_VIEW_H
