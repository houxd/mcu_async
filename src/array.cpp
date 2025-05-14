/*
 *  Copyright (C) 2024 by houxd
 *  All rights reserved.
 */
#include "array.h"
#include <stdio.h>
#include <string>

void _test_array() {
    // 基本构造测试
    Array<int, 5> a1;
    for (int i = 0; i < 5; i++) {
        a1[i] = i + 1;
    }
    
    printf("Array a1 elements: ");
    for (const auto& item : a1) {
        printf("%d ", item);
    }
    printf("\n");
    
    // 使用初始化列表构造
    Array<int, 5> a2 = {10, 20, 30};  // 后两个元素为0
    printf("Array a2 elements: ");
    for (const auto& item : a2) {
        printf("%d ", item);
    }
    printf("\n");
    
    // 使用make_array辅助函数
    auto a3 = make_array<int>(100, 200, 300, 400);
    printf("Array a3 elements: ");
    for (const auto& item : a3) {
        printf("%d ", item);
    }
    printf("\n");
    
    // 测试at函数和边界检查
    // try {
        printf("a2[2]: %d\n", a2.at(2));
        printf("a2[10]: "); // 这应该抛出异常
        a2.at(10);
    // } catch (const std::out_of_range& e) {
    //     printf("Exception: %s\n", e.what());
    // }
    
    // 测试查找和索引
    printf("a1 contains 3: %d\n", a1.contains(3));
    printf("a1 contains 10: %d\n", a1.contains(10));
    printf("Index of 3 in a1: %d\n", a1.index_of(3));
    printf("Index of 10 in a1: %d\n", a1.index_of(10));
    
    // 测试排序和反转
    Array<int, 5> a4 = {5, 2, 4, 1, 3};
    a4.sort();
    printf("Sorted a4: ");
    for (const auto& item : a4) {
        printf("%d ", item);
    }
    printf("\n");
    
    a4.reverse();
    printf("Reversed a4: ");
    for (const auto& item : a4) {
        printf("%d ", item);
    }
    printf("\n");
    
    // 测试filter和map
    Vec<int> filtered = a1.filter([](int x) { return x % 2 == 0; });
    printf("Filtered a1 (even numbers): ");
    for (const auto& item : filtered) {
        printf("%d ", item);
    }
    printf("\n");
    
    Vec<std::string> mapped = a1.map<std::string>([](int x) { return "Num" + std::to_string(x); });
    printf("Mapped a1: ");
    for (const auto& item : mapped) {
        printf("%s ", item.c_str());
    }
    printf("\n");
    
    // 测试填充
    Array<int, 5> a5;
    a5.fill(42);
    printf("a5 after fill(42): ");
    for (const auto& item : a5) {
        printf("%d ", item);
    }
    printf("\n");
    
    // 测试转换为Vec
    Vec<int> v1 = a1.to_vec();
    printf("Vec from a1: ");
    for (const auto& item : v1) {
        printf("%d ", item);
    }
    printf("\n");
    
    // 测试计数
    Array<int, 10> a6 = {1, 2, 2, 3, 2, 4, 5, 2, 6, 2};
    printf("Count of 2 in a6: %zu\n", a6.count_if(2));
    printf("Count of even numbers in a6: %zu\n", a6.count_if([](int x) { return x % 2 == 0; }));
}
