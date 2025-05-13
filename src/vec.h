/*
 *  Copyright (C) 2020 by houxd
 *  All rights reserved.
 */
#ifndef VEC_H
#define VEC_H

#include <vector>
#include <utility>

template <typename T> 
class Vec {
  private:
    std::vector<T> data;

  public:
    // 类型定义，与vector保持一致
    using value_type = typename std::vector<T>::value_type;
    using allocator_type = typename std::vector<T>::allocator_type;
    using size_type = typename std::vector<T>::size_type;
    using difference_type = typename std::vector<T>::difference_type;
    using reference = typename std::vector<T>::reference;
    using const_reference = typename std::vector<T>::const_reference;
    using pointer = typename std::vector<T>::pointer;
    using const_pointer = typename std::vector<T>::const_pointer;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    using reverse_iterator = typename std::vector<T>::reverse_iterator;
    using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;
    
    // 构造函数
    Vec() = default;
    explicit Vec(size_type count) : data(count) {}
    Vec(size_type count, const T& value) : data(count, value) {}
    template<typename InputIt>
    Vec(InputIt first, InputIt last) : data(first, last) {}
    Vec(const Vec& other) : data(other.data) {}
    Vec(Vec&& other) noexcept : data(std::move(other.data)) {}
    Vec(std::initializer_list<T> init) : data(init) {}
    
    // 赋值运算符
    Vec& operator=(const Vec& other) {
        data = other.data;
        return *this;
    }
    Vec& operator=(Vec&& other) noexcept {
        data = std::move(other.data);
        return *this;
    }
    Vec& operator=(std::initializer_list<T> ilist) {
        data = ilist;
        return *this;
    }
    
    // 元素访问
    reference at(size_type pos) { return data.at(pos); }
    const_reference at(size_type pos) const { return data.at(pos); }
    reference operator[](size_type pos) { return data[pos]; }
    const_reference operator[](size_type pos) const { return data[pos]; }
    reference front() { return data.front(); }
    const_reference front() const { return data.front(); }
    reference back() { return data.back(); }
    const_reference back() const { return data.back(); }
    T* data_ptr() { return data.data(); }
    const T* data_ptr() const { return data.data(); }
    
    // 迭代器
    iterator begin() noexcept { return data.begin(); }
    const_iterator begin() const noexcept { return data.begin(); }
    const_iterator cbegin() const noexcept { return data.cbegin(); }
    iterator end() noexcept { return data.end(); }
    const_iterator end() const noexcept { return data.end(); }
    const_iterator cend() const noexcept { return data.cend(); }
    reverse_iterator rbegin() noexcept { return data.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return data.rbegin(); }
    const_reverse_iterator crbegin() const noexcept { return data.crbegin(); }
    reverse_iterator rend() noexcept { return data.rend(); }
    const_reverse_iterator rend() const noexcept { return data.rend(); }
    const_reverse_iterator crend() const noexcept { return data.crend(); }
    
    // 容量
    bool empty() const noexcept { return data.empty(); }
    size_type size() const noexcept { return data.size(); }
    size_type max_size() const noexcept { return data.max_size(); }
    void reserve(size_type new_cap) { data.reserve(new_cap); }
    size_type capacity() const noexcept { return data.capacity(); }
    void shrink_to_fit() { data.shrink_to_fit(); }
    
    // 修改器
    void clear() noexcept { data.clear(); }
    iterator insert(const_iterator pos, const T& value) { return data.insert(pos, value); }
    iterator insert(const_iterator pos, T&& value) { return data.insert(pos, std::move(value)); }
    iterator insert(const_iterator pos, size_type count, const T& value) { return data.insert(pos, count, value); }
    template<typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last) { return data.insert(pos, first, last); }
    iterator insert(const_iterator pos, std::initializer_list<T> ilist) { return data.insert(pos, ilist); }
    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) { return data.emplace(pos, std::forward<Args>(args)...); }
    iterator erase(const_iterator pos) { return data.erase(pos); }
    iterator erase(const_iterator first, const_iterator last) { return data.erase(first, last); }
    void push_back(const T& value) { data.push_back(value); }
    void push_back(T&& value) { data.push_back(std::move(value)); }
    template<typename... Args>
    reference emplace_back(Args&&... args) { return data.emplace_back(std::forward<Args>(args)...); }
    void pop_back() { data.pop_back(); }
    void resize(size_type count) { data.resize(count); }
    void resize(size_type count, const value_type& value) { data.resize(count, value); }
    void swap(Vec& other) noexcept { data.swap(other.data); }
    
    // 非成员函数重载
    friend bool operator==(const Vec& lhs, const Vec& rhs) { return lhs.data == rhs.data; }
    friend bool operator!=(const Vec& lhs, const Vec& rhs) { return lhs.data != rhs.data; }
    friend bool operator<(const Vec& lhs, const Vec& rhs) { return lhs.data < rhs.data; }
    friend bool operator<=(const Vec& lhs, const Vec& rhs) { return lhs.data <= rhs.data; }
    friend bool operator>(const Vec& lhs, const Vec& rhs) { return lhs.data > rhs.data; }
    friend bool operator>=(const Vec& lhs, const Vec& rhs) { return lhs.data >= rhs.data; }
    
    // 你的自定义方法
    inline int count() const { return size(); }
    inline int length() const { return size(); }
    inline bool is_empty() const { return empty(); }
    inline void append(const T &lv) { push_back(lv); }
    inline void append(T &&rv) { push_back(std::move(rv)); }
    
    auto find(const T &lv) const {
        for (auto it = begin(); it != end(); ++it) {
            if (*it == lv)
                return it;
        }
        return end();
    }
    
    bool contains(const T &lv) const {
        for (const auto &v : *this) {
            if (v == lv)
                return true;
        }
        return false;
    }
    
    int index_of(const T &lv) const {
        for (unsigned i = 0; i < size(); i++) {
            if (at(i) == lv) {
                return i;
            }
        }
        return -1;
    }
    
    void remove(const T &lv) {
        auto it = find(lv);
        if (it != end()) {
            erase(it);
        }
    }
    
    void remove(int index) {
        if (index >= 0 && index < size()) {
            erase(begin() + index);
        }
    }

    Vec<T> filter(bool (*func)(const T &)) {
        Vec<T> ret;
        for (const auto &v : *this) {
            if (func(v)) {
                ret.append(v);
            }
        }
        return ret;
    }

    Vec<T> filter(bool (*func)(const T &, int)) {
        Vec<T> ret;
        for (unsigned i = 0; i < size(); i++) {
            if (func(at(i), i)) {
                ret.append(at(i));
            }
        }
        return ret;
    }
    
    template <typename R> 
    Vec<R> map(R (*func)(const T &)) {
        Vec<R> ret;
        for (const auto &v : *this) {
            ret.append(func(v));
        }
        return ret;
    }
    
    // 获取内部vector的引用（如果需要）
    const std::vector<T>& as_vector() const& { return data; }
    std::vector<T>& as_vector() & { return data; }
};

extern void _test_vec();

#endif // VEC_H
