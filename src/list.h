/*
 *  Copyright (C) 2024 by houxd
 *  All rights reserved.
 */
#ifndef LIST_H
#define LIST_H

#include <list>
#include <utility>
#include <functional>
#include <vec.h>
#include <assert.h>

template <typename T>
class List {
  private:
    std::list<T> data;

  public:
    // 类型定义，与std::list保持一致
    using value_type = typename std::list<T>::value_type;
    using allocator_type = typename std::list<T>::allocator_type;
    using size_type = typename std::list<T>::size_type;
    using difference_type = typename std::list<T>::difference_type;
    using reference = typename std::list<T>::reference;
    using const_reference = typename std::list<T>::const_reference;
    using pointer = typename std::list<T>::pointer;
    using const_pointer = typename std::list<T>::const_pointer;
    using iterator = typename std::list<T>::iterator;
    using const_iterator = typename std::list<T>::const_iterator;
    using reverse_iterator = typename std::list<T>::reverse_iterator;
    using const_reverse_iterator = typename std::list<T>::const_reverse_iterator;
    
    // 构造函数
    List() = default;
    explicit List(size_type count) : data(count) {}
    List(size_type count, const T& value) : data(count, value) {}
    template<typename InputIt>
    List(InputIt first, InputIt last) : data(first, last) {}
    List(const List& other) : data(other.data) {}
    List(List&& other) noexcept : data(std::move(other.data)) {}
    List(std::initializer_list<T> init) : data(init) {}
    
    // 赋值运算符
    List& operator=(const List& other) {
        data = other.data;
        return *this;
    }
    List& operator=(List&& other) noexcept {
        data = std::move(other.data);
        return *this;
    }
    List& operator=(std::initializer_list<T> ilist) {
        data = ilist;
        return *this;
    }
    
    // 元素访问
    reference front() { return data.front(); }
    const_reference front() const { return data.front(); }
    reference back() { return data.back(); }
    const_reference back() const { return data.back(); }
    
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
    void push_front(const T& value) { data.push_front(value); }
    void push_front(T&& value) { data.push_front(std::move(value)); }
    template<typename... Args>
    reference emplace_front(Args&&... args) { return data.emplace_front(std::forward<Args>(args)...); }
    void pop_front() { data.pop_front(); }
    void resize(size_type count) { data.resize(count); }
    void resize(size_type count, const value_type& value) { data.resize(count, value); }
    void swap(List& other) noexcept { data.swap(other.data); }
    
    // 操作
    void merge(List& other) { data.merge(other.data); }
    template<typename Compare>
    void merge(List& other, Compare comp) { data.merge(other.data, comp); }
    void merge(List&& other) { data.merge(std::move(other.data)); }
    template<typename Compare>
    void merge(List&& other, Compare comp) { data.merge(std::move(other.data), comp); }
    void splice(const_iterator pos, List& other) { data.splice(pos, other.data); }
    void splice(const_iterator pos, List&& other) { data.splice(pos, std::move(other.data)); }
    void splice(const_iterator pos, List& other, const_iterator it) { data.splice(pos, other.data, it); }
    void splice(const_iterator pos, List&& other, const_iterator it) { data.splice(pos, std::move(other.data), it); }
    void splice(const_iterator pos, List& other, const_iterator first, const_iterator last) { data.splice(pos, other.data, first, last); }
    void splice(const_iterator pos, List&& other, const_iterator first, const_iterator last) { data.splice(pos, std::move(other.data), first, last); }
    size_type remove(const T& value) { return data.remove(value); }
    template<typename UnaryPredicate>
    size_type remove_if(UnaryPredicate p) { return data.remove_if(p); }
    void reverse() noexcept { data.reverse(); }
    void unique() { data.unique(); }
    template<typename BinaryPredicate>
    void unique(BinaryPredicate p) { data.unique(p); }
    void sort() { data.sort(); }
    template<typename Compare>
    void sort(Compare comp) { data.sort(comp); }
    
    // 非成员函数重载
    friend bool operator==(const List& lhs, const List& rhs) { return lhs.data == rhs.data; }
    friend bool operator!=(const List& lhs, const List& rhs) { return lhs.data != rhs.data; }
    friend bool operator<(const List& lhs, const List& rhs) { return lhs.data < rhs.data; }
    friend bool operator<=(const List& lhs, const List& rhs) { return lhs.data <= rhs.data; }
    friend bool operator>(const List& lhs, const List& rhs) { return lhs.data > rhs.data; }
    friend bool operator>=(const List& lhs, const List& rhs) { return lhs.data >= rhs.data; }
    
    // 自定义便捷方法
    inline int count_items() const { return size(); }
    inline bool is_empty() const { return empty(); }
    
    // 添加元素的便捷方法
    void append(const T& value) { push_back(value); }
    void append(T&& value) { push_back(std::move(value)); }
    void prepend(const T& value) { push_front(value); }
    void prepend(T&& value) { push_front(std::move(value)); }
    
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
    
    // 获取指定索引的元素（注意：这是O(n)操作）
    T& at(size_type index) {
        if (index >= size()) {
            assert(false);
            //throw std::out_of_range("List::at: index out of range");
        }
        auto it = begin();
        std::advance(it, index);
        return *it;
    }
    
    const T& at(size_type index) const {
        if (index >= size()) {
            assert(false);
            //throw std::out_of_range("List::at: index out of range");
        }
        auto it = begin();
        std::advance(it, index);
        return *it;
    }
    
    // 在指定索引位置插入元素
    iterator insert_at(size_type index, const T& value) {
        auto it = begin();
        std::advance(it, index);
        return insert(it, value);
    }
    
    // 移除指定索引的元素
    iterator erase_at(size_type index) {
        auto it = begin();
        std::advance(it, index);
        return erase(it);
    }
    
    // 转换为Vec
    Vec<T> to_vec() const {
        return Vec<T>(begin(), end());
    }
    
    // 过滤
    template<typename Func>
    List<T> filter(Func predicate) const {
        List<T> result;
        for (const auto& item : data) {
            if (predicate(item)) {
                result.push_back(item);
            }
        }
        return result;
    }
    
    // 映射
    template<typename R, typename Func>
    List<R> map(Func mapper) const {
        List<R> result;
        for (const auto& item : data) {
            result.push_back(mapper(item));
        }
        return result;
    }
    
    // 获取内部std::list的引用
    const std::list<T>& as_std_list() const& { return data; }
    std::list<T>& as_std_list() & { return data; }
};

extern void _test_list();

#endif // LIST_H
