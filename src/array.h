/*
 *  Copyright (C) 2024 by houxd
 *  All rights reserved.
 */
#ifndef ARRAY_H
#define ARRAY_H

#include <array>
#include <utility>
#include <algorithm>
#include <initializer_list>
#include <vec.h>
#include <cassert>

template <typename T, size_t N>
class Array {
  private:
    std::array<T, N> data;

  public:
    // 类型定义，与std::array保持一致
    using value_type = typename std::array<T, N>::value_type;
    using size_type = typename std::array<T, N>::size_type;
    using difference_type = typename std::array<T, N>::difference_type;
    using reference = typename std::array<T, N>::reference;
    using const_reference = typename std::array<T, N>::const_reference;
    using pointer = typename std::array<T, N>::pointer;
    using const_pointer = typename std::array<T, N>::const_pointer;
    using iterator = typename std::array<T, N>::iterator;
    using const_iterator = typename std::array<T, N>::const_iterator;
    using reverse_iterator = typename std::array<T, N>::reverse_iterator;
    using const_reverse_iterator = typename std::array<T, N>::const_reverse_iterator;
    
    // 构造函数
    Array() = default;
    Array(const Array& other) = default;
    Array(Array&& other) = default;
    
    // 从初始化列表构造
    Array(std::initializer_list<T> init) {
        size_t count = std::min(init.size(), N);
        std::copy_n(init.begin(), count, data.begin());
        
        // 如果初始化列表小于N，用默认值填充
        if (count < N) {
            std::fill(data.begin() + count, data.end(), T());
        }
    }

    Array(const std::array<T,N>& d): data(d) {}
    Array(std::array<T,N>&& d): data(std::move(d)) {}
    // 访问内部std::array
    const std::array<T, N>& as_std_array() const& { return data; }
    std::array<T, N>& as_std_array() & { return data; }
    
    // 从另一种类型的Array构造
    template<typename U, size_t M>
    explicit Array(const Array<U, M>& other) {
        size_t count = std::min(other.size(), N);
        for (size_t i = 0; i < count; ++i) {
            data[i] = static_cast<T>(other[i]);
        }
        // 如果other的大小小于N，用默认值填充
        if (count < N) {
            std::fill(data.begin() + count, data.end(), T());
        }
    }
    
    // 从C风格数组构造
    explicit Array(const T (&arr)[N]) {
        std::copy_n(arr, N, data.begin());
    }
    
    // 赋值运算符
    Array& operator=(const Array& other) = default;
    Array& operator=(Array&& other) = default;
    Array& operator=(std::initializer_list<T> ilist) {
        size_t count = std::min(ilist.size(), N);
        std::copy_n(ilist.begin(), count, data.begin());
        // 如果初始化列表小于N，用默认值填充
        if (count < N) {
            std::fill(data.begin() + count, data.end(), T());
        }
        return *this;
    }
    
    // 元素访问
    reference at(size_type pos) {
        assert(pos >= N);
        return data[pos];
    }
    const_reference at(size_type pos) const {
        assert(pos >= N);
        return data[pos];
    }
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
    
    // 容量相关
    bool empty() const noexcept { return N == 0; }
    size_type size() const noexcept { return N; }
    size_type max_size() const noexcept { return N; }
    
    // 修改器
    void fill(const T& value) { data.fill(value); }
    void swap(Array& other) noexcept { data.swap(other.data); }
    
    // 非成员函数重载
    friend bool operator==(const Array& lhs, const Array& rhs) { return lhs.data == rhs.data; }
    friend bool operator!=(const Array& lhs, const Array& rhs) { return lhs.data != rhs.data; }
    friend bool operator<(const Array& lhs, const Array& rhs) { return lhs.data < rhs.data; }
    friend bool operator<=(const Array& lhs, const Array& rhs) { return lhs.data <= rhs.data; }
    friend bool operator>(const Array& lhs, const Array& rhs) { return lhs.data > rhs.data; }
    friend bool operator>=(const Array& lhs, const Array& rhs) { return lhs.data >= rhs.data; }
    
    // 与Vec类似的接口
    inline int count() const { return size(); }
    inline int length() const { return size(); }
    inline bool is_empty() const { return empty(); }
    
    // 查找元素
    iterator find(const T& value) {
        return std::find(begin(), end(), value);
    }
    const_iterator find(const T& value) const {
        return std::find(begin(), end(), value);
    }
    
    // 判断是否包含元素
    bool contains(const T& value) const {
        return std::find(begin(), end(), value) != end();
    }
    
    // 查找元素的索引，不存在返回-1
    int index_of(const T& value) const {
        for (size_t i = 0; i < size(); i++) {
            if (data[i] == value) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
    
    // 转换为Vec
    Vec<T> to_vec() const {
        return Vec<T>(begin(), end());
    }
    
    // 过滤元素创建Vec
    template<typename Func>
    Vec<T> filter(Func predicate) const {
        Vec<T> result;
        for (const auto& item : data) {
            if (predicate(item)) {
                result.push_back(item);
            }
        }
        return result;
    }
    
    // 映射元素创建Vec
    template<typename R, typename Func>
    Vec<R> map(Func mapper) const {
        Vec<R> result;
        for (const auto& item : data) {
            result.push_back(mapper(item));
        }
        return result;
    }
    
    // 排序数组（原地修改）
    void sort() {
        std::sort(begin(), end());
    }
    
    template<typename Compare>
    void sort(Compare comp) {
        std::sort(begin(), end(), comp);
    }
    
    // 反转数组（原地修改）
    void reverse() {
        std::reverse(begin(), end());
    }
    
    // 元素计数
    size_t count_if(const T& value) const {
        return std::count(begin(), end(), value);
    }
    
    template<typename Pred>
    size_t count_if(Pred predicate) const {
        return std::count_if(begin(), end(), predicate);
    }

};

// 辅助函数：创建Array的便捷方法
template<typename T, typename... Args>
auto make_array(Args&&... args) -> Array<T, sizeof...(Args)> {
    return Array<T, sizeof...(Args)>({static_cast<T>(std::forward<Args>(args))...});
}

extern void _test_array();

#endif // ARRAY_H
