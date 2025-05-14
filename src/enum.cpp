#include <enum.h>

void _test_enum()
{
    printf("Test Enum\n");
    enum class Color {
        invalid,
        red,
        green,
        blue,
    };

    auto values  = enum_values<Color>();
    assert(values.map<Str>([](auto e){return Str::from_int((int)e);}).join(", ") == "0, 1, 2, 3");

    auto names = enum_names<Color>();
    assert(names.map<Str>([](auto e){return Str(e);}).join(", ") == "invalid, red, green, blue");

    auto entries = enum_entries<Color>();
    assert(
        entries.map<Str>([](auto e){
                   return Str::sprintf("(%d,%s)", (int)e.first, Str(e.second).c_str());
               }).join(", ")
        == "(0,invalid), (1,red), (2,green), (3,blue)"
    );

    assert((int)enum_count<Color>()==4);
    assert(enum_str(Color::red)=="red");
    assert(enum_cast("red", Color::invalid)==Color::red);
    printf("Test Enum PASS\n");
}
