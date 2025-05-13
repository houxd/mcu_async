#include <types.h>
#include <stdio.h>
#include <assert.h>

void _test_types() {
    // Test enum2str and str2enum
    enum class Color { RED, GREEN, BLUE };
    Color color = Color::GREEN;
    Str color_str = enum2str(color);
    Color color_from_str = str2enum(color_str.c_str(), Color::RED);
    if (color_from_str == Color::GREEN) {
        printf("Enum Test Success: %s\n", color_str.c_str());
    } else {
        printf("Enum Test Failure: %s\n", color_str.c_str());
        assert(false);
    }

    // Test Unique, Shared, and Weak pointers
    Unique<int> unique_ptr = make_unique<int>(42);
    Shared<int> shared_ptr = make_shared<int>(42);
    Weak<int> weak_ptr = shared_ptr;
    if (auto locked_ptr = weak_ptr.lock()) {
        printf("Weak Pointer Test Success: %d\n", *locked_ptr);
    } else {
        printf("Weak Pointer Test Failure\n");
        assert(false);
    }

    // Test Func
    Func<void(int)> func = [](int x) {
        printf("Func Test Success: %d\n", x);
    };
    func(42);
}
