/*
 *  Copyright (C) 2024 by houxd
 *  All rights reserved.
 */
#include "list.h"
#include <stdio.h>
#include <string>

void _test_list() {
    // 基本操作测试
    List<int> l1;
    l1.append(1);
    l1.append(2);
    l1.append(3);
    l1.prepend(0);
    
    printf("List l1 size: %d\n", (int)l1.size());
    printf("List l1 front: %d\n", l1.front());
    printf("List l1 back: %d\n", l1.back());
    
    // 遍历列表
    printf("List l1 elements: ");
    for (const auto& element : l1) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 查找元素
    printf("List l1 contains 2: %d\n", l1.contains(2));
    printf("List l1 contains 5: %d\n", l1.contains(5));
    
    // 访问指定索引
    printf("List l1 at index 2: %d\n", l1.at(2));
    
    // 在指定位置插入
    l1.insert_at(2, 99);
    printf("List l1 after inserting 99 at position 2: ");
    for (const auto& element : l1) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 移除指定位置的元素
    l1.erase_at(2);
    printf("List l1 after removing element at position 2: ");
    for (const auto& element : l1) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 移除指定值的元素
    l1.remove(2);
    printf("List l1 after removing value 2: ");
    for (const auto& element : l1) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 排序
    List<int> l2 = {5, 2, 8, 1, 3};
    l2.sort();
    printf("List l2 after sorting: ");
    for (const auto& element : l2) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 反转
    l2.reverse();
    printf("List l2 after reversing: ");
    for (const auto& element : l2) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 过滤
    auto l3 = l2.filter([](int x) { return x > 2; });
    printf("Filtered list: ");
    for (const auto& element : l3) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 映射
    auto l4 = l2.map<std::string>([](int x) { return "Num" + std::to_string(x); });
    printf("Mapped list: ");
    for (const auto& element : l4) {
        printf("%s ", element.c_str());
    }
    printf("\n");
    
    // 合并列表
    List<int> l5 = {10, 20, 30};
    List<int> l6 = {15, 25, 35};
    l5.merge(l6);
    printf("Merged list: ");
    for (const auto& element : l5) {
        printf("%d ", element);
    }
    printf("\n");
    printf("Original l6 after merging: %d\n", (int)l6.size());
    
    // 截取部分
    List<int> l7 = {1, 2, 3, 4, 5};
    List<int> l8 = {10, 20, 30};
    auto it_begin = l7.begin();
    std::advance(it_begin, 1);
    auto it_end = l7.begin();
    std::advance(it_end, 4);
    
    l8.splice(l8.begin(), l7, it_begin, it_end);
    printf("l8 after splicing: ");
    for (const auto& element : l8) {
        printf("%d ", element);
    }
    printf("\n");
    printf("l7 after splicing: ");
    for (const auto& element : l7) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 转换为Vec
    Vec<int> v1 = l8.to_vec();
    printf("Vec from list: ");
    for (const auto& element : v1) {
        printf("%d ", element);
    }
    printf("\n");
}
