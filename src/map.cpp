/*
 *  Copyright (C) 2024 by houxd
 *  All rights reserved.
 */
#include "map.h"
#include <stdio.h>
#include <string>

void _test_map() {
    // 基本操作测试
    Map<std::string, int> m1;
    m1.put("one", 1);
    m1.put("two", 2);
    m1.put("three", 3);
    
    printf("Map m1 size: %d\n", (int)m1.size());
    printf("Map m1 contains 'two': %d\n", m1.contains("two"));
    printf("Map m1 contains 'four': %d\n", m1.contains("four"));
    
    // 通过下标访问
    printf("Map m1['one']: %d\n", m1["one"]);
    m1["one"] = 100;
    printf("Map m1['one'] after update: %d\n", m1["one"]);
    
    // 添加新元素通过[]
    m1["four"] = 4;
    printf("Map m1 contains 'four' after adding: %d\n", m1.contains("four"));
    
    // 遍历Map
    printf("Map m1 elements:\n");
    for (const auto& pair : m1) {
        printf("  %s: %d\n", pair.first.c_str(), pair.second);
    }
    
    // 删除元素
    m1.remove("two");
    printf("Map m1 contains 'two' after removing: %d\n", m1.contains("two"));
    printf("Map m1 size after removing: %d\n", (int)m1.size());
    
    // 获取所有键
    Vec<std::string> keys = m1.keys();
    printf("Map m1 keys: ");
    for (const auto& key : keys) {
        printf("%s ", key.c_str());
    }
    printf("\n");
    
    // 获取所有值
    Vec<int> values = m1.values();
    printf("Map m1 values: ");
    for (const auto& value : values) {
        printf("%d ", value);
    }
    printf("\n");
    
    // 过滤
    auto filtered = m1.filter([](const std::string& key, int value) {
        (void)key;
        return value > 3;
    });
    printf("Filtered map size: %d\n", (int)filtered.size());
    printf("Filtered map elements:\n");
    for (const auto& pair : filtered) {
        printf("  %s: %d\n", pair.first.c_str(), pair.second);
    }
    
    // 值映射
    auto mapped = m1.map_values<std::string>([](int value) {
        return std::to_string(value * 10);
    });
    printf("Mapped map elements:\n");
    for (const auto& pair : mapped) {
        printf("  %s: %s\n", pair.first.c_str(), pair.second.c_str());
    }
    
    // 初始化列表构造
    Map<int, std::string> m2 = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };
    printf("Map m2 size: %d\n", (int)m2.size());
    printf("Map m2[2]: %s\n", m2[2].c_str());
}
