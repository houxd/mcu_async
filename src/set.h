/*
 *  Copyright (C) 2024 by houxd
 *  All rights reserved.
 */
#ifndef SET_H
#define SET_H

#include <set>
#include <utility>
#include <algorithm>
#include <vec.h>

template <typename T, typename Compare = std::less<T>> 
class Set {
  private:
    std::set<T, Compare> data;

  public:
    // 类型定义，与 std::set 保持一致
    using key_type = typename std::set<T, Compare>::key_type;
    using value_type = typename std::set<T, Compare>::value_type;
    using size_type = typename std::set<T, Compare>::size_type;
    using difference_type = typename std::set<T, Compare>::difference_type;
    using key_compare = typename std::set<T, Compare>::key_compare;
    using value_compare = typename std::set<T, Compare>::value_compare;
    using reference = typename std::set<T, Compare>::reference;
    using const_reference = typename std::set<T, Compare>::const_reference;
    using pointer = typename std::set<T, Compare>::pointer;
    using const_pointer = typename std::set<T, Compare>::const_pointer;
    using iterator = typename std::set<T, Compare>::iterator;
    using const_iterator = typename std::set<T, Compare>::const_iterator;
    using reverse_iterator = typename std::set<T, Compare>::reverse_iterator;
    using const_reverse_iterator = typename std::set<T, Compare>::const_reverse_iterator;
    
    // 构造函数
    Set() = default;
    explicit Set(const Compare& comp) : data(comp) {}
    template<typename InputIt>
    Set(InputIt first, InputIt last, const Compare& comp = Compare()) : data(first, last, comp) {}
    Set(const Set& other) : data(other.data) {}
    Set(Set&& other) noexcept : data(std::move(other.data)) {}
    Set(std::initializer_list<T> init, const Compare& comp = Compare()) : data(init, comp) {}

    Set(const std::set<T>& d): data(d) {}
    Set(std::set<T>&& d): data(std::move(d)) {}
    // 获取内部 std::set 的引用
    const std::set<T, Compare>& as_std_set() const& { return data; }
    std::set<T, Compare>& as_std_set() & { return data; }

    // 赋值运算符
    Set& operator=(const Set& other) {
        data = other.data;
        return *this;
    }
    Set& operator=(Set&& other) noexcept {
        data = std::move(other.data);
        return *this;
    }
    Set& operator=(std::initializer_list<T> ilist) {
        data = ilist;
        return *this;
    }
    
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
    
    // 修改器
    void clear() noexcept { data.clear(); }
    std::pair<iterator, bool> insert(const T& value) { return data.insert(value); }
    std::pair<iterator, bool> insert(T&& value) { return data.insert(std::move(value)); }
    iterator insert(const_iterator hint, const T& value) { return data.insert(hint, value); }
    iterator insert(const_iterator hint, T&& value) { return data.insert(hint, std::move(value)); }
    template<typename InputIt>
    void insert(InputIt first, InputIt last) { data.insert(first, last); }
    void insert(std::initializer_list<T> ilist) { data.insert(ilist); }
    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) { return data.emplace(std::forward<Args>(args)...); }
    template<typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args) { return data.emplace_hint(hint, std::forward<Args>(args)...); }
    iterator erase(const_iterator pos) { return data.erase(pos); }
    iterator erase(const_iterator first, const_iterator last) { return data.erase(first, last); }
    size_type erase(const key_type& key) { return data.erase(key); }
    void swap(Set& other) noexcept { data.swap(other.data); }
    
    // 查找
    size_type count(const T& key) const { return data.count(key); }
    iterator find(const T& key) { return data.find(key); }
    const_iterator find(const T& key) const { return data.find(key); }
    std::pair<iterator, iterator> equal_range(const T& key) { return data.equal_range(key); }
    std::pair<const_iterator, const_iterator> equal_range(const T& key) const { return data.equal_range(key); }
    iterator lower_bound(const T& key) { return data.lower_bound(key); }
    const_iterator lower_bound(const T& key) const { return data.lower_bound(key); }
    iterator upper_bound(const T& key) { return data.upper_bound(key); }
    const_iterator upper_bound(const T& key) const { return data.upper_bound(key); }
    
    // 观察器
    key_compare key_comp() const { return data.key_comp(); }
    value_compare value_comp() const { return data.value_comp(); }
    
    // 非成员函数重载
    friend bool operator==(const Set& lhs, const Set& rhs) { return lhs.data == rhs.data; }
    friend bool operator!=(const Set& lhs, const Set& rhs) { return lhs.data != rhs.data; }
    friend bool operator<(const Set& lhs, const Set& rhs) { return lhs.data < rhs.data; }
    friend bool operator<=(const Set& lhs, const Set& rhs) { return lhs.data <= rhs.data; }
    friend bool operator>(const Set& lhs, const Set& rhs) { return lhs.data > rhs.data; }
    friend bool operator>=(const Set& lhs, const Set& rhs) { return lhs.data >= rhs.data; }
    
    // 自定义便捷方法
    inline int count_items() const { return size(); }
    inline bool is_empty() const { return empty(); }
    
    // 添加元素的便捷方法
    void add(const T& value) { insert(value); }
    void add(T&& value) { insert(std::move(value)); }
    
    // 判断是否包含元素
    bool contains(const T& value) const {
        return data.find(value) != data.end();
    }
    
    // 移除元素
    void remove(const T& value) {
        data.erase(value);
    }
    
    // 与另一个集合的并集操作
    Set<T> union_with(const Set<T>& other) const {
        Set<T> result = *this;
        result.data.insert(other.begin(), other.end());
        return result;
    }
    
    // 与另一个集合的交集操作
    Set<T> intersection_with(const Set<T>& other) const {
        Set<T> result;
        for (const auto& item : data) {
            if (other.contains(item)) {
                result.add(item);
            }
        }
        return result;
    }
    
    // 与另一个集合的差集操作 (this - other)
    Set<T> difference_with(const Set<T>& other) const {
        Set<T> result;
        for (const auto& item : data) {
            if (!other.contains(item)) {
                result.add(item);
            }
        }
        return result;
    }
    
    // 是否为另一个集合的子集
    bool is_subset_of(const Set<T>& other) const {
        if (size() > other.size()) {
            return false;
        }
        
        for (const auto& item : data) {
            if (!other.contains(item)) {
                return false;
            }
        }
        return true;
    }
    
    // 是否为另一个集合的超集
    bool is_superset_of(const Set<T>& other) const {
        return other.is_subset_of(*this);
    }
    
    // 将集合转换为Vec
    Vec<T> to_vec() const {
        Vec<T> result;
        for (const auto& item : data) {
            result.push_back(item);
        }
        return result;
    }
    
    // 通过过滤函数创建新集合
    template<typename Func>
    Set<T> filter(Func predicate) const {
        Set<T> result;
        for (const auto& item : data) {
            if (predicate(item)) {
                result.add(item);
            }
        }
        return result;
    }
    
    // 通过映射函数创建新类型的集合
    template<typename R, typename Func>
    Set<R> map(Func mapper) const {
        Set<R> result;
        for (const auto& item : data) {
            result.add(mapper(item));
        }
        return result;
    }

};

extern void _test_set();

#endif // SET_H
