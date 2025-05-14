#include "set.h"
#include "vec.h"
#include <stdio.h>
#include <string>

void _test_set() {
    // 基本操作测试
    Set<int> s1;
    s1.add(1);
    s1.add(2);
    s1.add(3);
    s1.add(1); // 重复元素不会被添加
    
    printf("Set s1 size: %d\n", (int)s1.size());
    printf("Set s1 contains 2: %d\n", s1.contains(2));
    printf("Set s1 contains 4: %d\n", s1.contains(4));
    
    // 遍历集合
    printf("Set s1 elements: ");
    for (const auto& element : s1) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 删除元素
    s1.remove(2);
    printf("After removing 2, Set s1 contains 2: %d\n", s1.contains(2));
    printf("After removing 2, Set s1 size: %d\n", (int)s1.size());
    
    // 测试集合操作
    Set<int> s2 = {2, 3, 4, 5};
    
    // 并集
    Set<int> union_set = s1.union_with(s2);
    printf("Union set size: %d\n", (int)union_set.size());
    printf("Union set elements: ");
    for (const auto& element : union_set) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 交集
    Set<int> intersection_set = s1.intersection_with(s2);
    printf("Intersection set size: %d\n", (int)intersection_set.size());
    printf("Intersection set elements: ");
    for (const auto& element : intersection_set) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 差集
    Set<int> difference_set = s1.difference_with(s2);
    printf("Difference set size: %d\n", (int)difference_set.size());
    printf("Difference set elements: ");
    for (const auto& element : difference_set) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 测试子集与超集
    Set<int> subset = {1, 3};
    printf("subset is subset of s1: %d\n", subset.is_subset_of(s1));
    printf("s1 is superset of subset: %d\n", s1.is_superset_of(subset));
    
    // 测试过滤函数
    Set<int> filtered = s2.filter([](int x) { return x % 2 == 0; }); // 只保留偶数
    printf("Filtered set elements: ");
    for (const auto& element : filtered) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 测试映射函数
    Set<int> mapped = s2.map<int>([](int x) { return x * x; }); // 求平方
    printf("Mapped set elements: ");
    for (const auto& element : mapped) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 测试转换为Vec
    Vec<int> vec = s1.to_vec();
    printf("Vec from set elements: ");
    for (const auto& element : vec) {
        printf("%d ", element);
    }
    printf("\n");
    
    // 测试字符串Set
    Set<std::string> str_set;
    str_set.add("hello");
    str_set.add("world");
    str_set.add("hello"); // 重复元素
    
    printf("String set size: %d\n", (int)str_set.size());
    printf("String set contains 'hello': %d\n", str_set.contains("hello"));
    printf("String set contains 'test': %d\n", str_set.contains("test"));
}


