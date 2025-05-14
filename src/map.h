/*
 *  Copyright (C) 2024 by houxd
 *  All rights reserved.
 */
#ifndef MAP_H
#define MAP_H

#include <map>
#include <utility>
#include <vec.h>

template <typename K, typename V, typename Compare = std::less<K>>
class Map {
  private:
    std::map<K, V, Compare> data;

  public:
    // 类型定义，与std::map保持一致
    using key_type = typename std::map<K, V, Compare>::key_type;
    using mapped_type = typename std::map<K, V, Compare>::mapped_type;
    using value_type = typename std::map<K, V, Compare>::value_type;
    using size_type = typename std::map<K, V, Compare>::size_type;
    using difference_type = typename std::map<K, V, Compare>::difference_type;
    using key_compare = typename std::map<K, V, Compare>::key_compare;
    using value_compare = typename std::map<K, V, Compare>::value_compare;
    using reference = typename std::map<K, V, Compare>::reference;
    using const_reference = typename std::map<K, V, Compare>::const_reference;
    using pointer = typename std::map<K, V, Compare>::pointer;
    using const_pointer = typename std::map<K, V, Compare>::const_pointer;
    using iterator = typename std::map<K, V, Compare>::iterator;
    using const_iterator = typename std::map<K, V, Compare>::const_iterator;
    using reverse_iterator = typename std::map<K, V, Compare>::reverse_iterator;
    using const_reverse_iterator = typename std::map<K, V, Compare>::const_reverse_iterator;
    
    // 构造函数
    Map() = default;
    explicit Map(const Compare& comp) : data(comp) {}
    template<typename InputIt>
    Map(InputIt first, InputIt last, const Compare& comp = Compare()) : data(first, last, comp) {}
    Map(const Map& other) : data(other.data) {}
    Map(Map&& other) noexcept : data(std::move(other.data)) {}
    Map(std::initializer_list<value_type> init, const Compare& comp = Compare()) : data(init, comp) {}

    Map(const std::map<K,V>& d): data(d) {}
    Map(std::map<K,V>&& d): data(std::move(d)) {}
    // 获取内部std::map的引用
    const std::map<K, V, Compare>& std_map() const& { return data; }
    std::map<K, V, Compare>& std_map() & { return data; }

    // 赋值运算符
    Map& operator=(const Map& other) {
        data = other.data;
        return *this;
    }
    Map& operator=(Map&& other) noexcept {
        data = std::move(other.data);
        return *this;
    }
    Map& operator=(std::initializer_list<value_type> ilist) {
        data = ilist;
        return *this;
    }
    
    // 元素访问
    mapped_type& at(const key_type& key) { return data.at(key); }
    const mapped_type& at(const key_type& key) const { return data.at(key); }
    mapped_type& operator[](const key_type& key) { return data[key]; }
    mapped_type& operator[](key_type&& key) { return data[std::move(key)]; }
    
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
    std::pair<iterator, bool> insert(const value_type& value) { return data.insert(value); }
    std::pair<iterator, bool> insert(value_type&& value) { return data.insert(std::move(value)); }
    iterator insert(const_iterator hint, const value_type& value) { return data.insert(hint, value); }
    iterator insert(const_iterator hint, value_type&& value) { return data.insert(hint, std::move(value)); }
    template<typename InputIt>
    void insert(InputIt first, InputIt last) { data.insert(first, last); }
    void insert(std::initializer_list<value_type> ilist) { data.insert(ilist); }
    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) { return data.emplace(std::forward<Args>(args)...); }
    template<typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args) { return data.emplace_hint(hint, std::forward<Args>(args)...); }
    iterator erase(const_iterator pos) { return data.erase(pos); }
    iterator erase(const_iterator first, const_iterator last) { return data.erase(first, last); }
    size_type erase(const key_type& key) { return data.erase(key); }
    void swap(Map& other) noexcept { data.swap(other.data); }
    
    // 查找
    iterator find(const K& key) { return data.find(key); }
    const_iterator find(const K& key) const { return data.find(key); }
    size_type count(const K& key) const { return data.count(key); }
    std::pair<iterator, iterator> equal_range(const K& key) { return data.equal_range(key); }
    std::pair<const_iterator, const_iterator> equal_range(const K& key) const { return data.equal_range(key); }
    iterator lower_bound(const K& key) { return data.lower_bound(key); }
    const_iterator lower_bound(const K& key) const { return data.lower_bound(key); }
    iterator upper_bound(const K& key) { return data.upper_bound(key); }
    const_iterator upper_bound(const K& key) const { return data.upper_bound(key); }
    
    // 观察器
    key_compare key_comp() const { return data.key_comp(); }
    value_compare value_comp() const { return data.value_comp(); }
    
    // 非成员函数重载
    friend bool operator==(const Map& lhs, const Map& rhs) { return lhs.data == rhs.data; }
    friend bool operator!=(const Map& lhs, const Map& rhs) { return lhs.data != rhs.data; }
    friend bool operator<(const Map& lhs, const Map& rhs) { return lhs.data < rhs.data; }
    friend bool operator<=(const Map& lhs, const Map& rhs) { return lhs.data <= rhs.data; }
    friend bool operator>(const Map& lhs, const Map& rhs) { return lhs.data > rhs.data; }
    friend bool operator>=(const Map& lhs, const Map& rhs) { return lhs.data >= rhs.data; }
    
    // 自定义便捷方法
    inline int count_items() const { return size(); }
    inline bool is_empty() const { return empty(); }
    
    // 判断是否包含键
    bool contains(const K& key) const {
        return data.find(key) != data.end();
    }
    
    // 使用键值对添加元素
    void put(const K& key, const V& value) {
        data[key] = value;
    }
    
    void put(const K& key, V&& value) {
        data[key] = std::move(value);
    }
    
    // 移除元素
    void remove(const K& key) {
        data.erase(key);
    }
    
    // 获取所有键
    Vec<K> keys() const {
        Vec<K> result;
        for (const auto& pair : data) {
            result.push_back(pair.first);
        }
        return result;
    }
    
    // 获取所有值
    Vec<V> values() const {
        Vec<V> result;
        for (const auto& pair : data) {
            result.push_back(pair.second);
        }
        return result;
    }
    
    // 通过过滤函数创建新映射
    template<typename Func>
    Map<K, V, Compare> filter(Func predicate) const {
        Map<K, V, Compare> result;
        for (const auto& pair : data) {
            if (predicate(pair.first, pair.second)) {
                result.put(pair.first, pair.second);
            }
        }
        return result;
    }
    
    // 通过映射函数转换值
    template<typename R, typename Func>
    Map<K, R, Compare> map_values(Func mapper) const {
        Map<K, R, Compare> result;
        for (const auto& pair : data) {
            result.put(pair.first, mapper(pair.second));
        }
        return result;
    }

};

extern void _test_map();

#endif // MAP_H
