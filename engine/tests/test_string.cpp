
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>


#include "frog/utils/string.hpp"

TEST_CASE("split")
{
    using str_vec = std::vector<std::string>;

    REQUIRE(frog::split("a,b,c", ",") == str_vec{ "a", "b", "c" });
    REQUIRE(frog::split("a,b,", ",") == str_vec{ "a", "b", "" });
    REQUIRE(frog::split(",b,", ",") == str_vec{ "", "b", "" });
    REQUIRE(frog::split("a,,c", ",") == str_vec{ "a", "", "c" });
}

TEST_CASE("split")
{
    auto check_s = [](std::string str, std::string_view expected)
    {
        frog::trim(str);
        REQUIRE_EQ(str, expected);
    };

    check_s("ahoj", "ahoj");
    check_s(" ahoj", "ahoj");
    check_s("ahoj ", "ahoj");
    check_s("", "");
    check_s(" ahoj ", "ahoj");
    check_s("   a h o j   ", "a h o j");

    auto check_sv = [](std::string_view str, std::string_view expected)
    {
        frog::trim(str);
        REQUIRE_EQ(str, expected);
    };
    check_sv("ahoj", "ahoj");
    check_sv(" ahoj", "ahoj");
    check_sv("ahoj ", "ahoj");
    check_sv("", "");
    check_sv(" ahoj ", "ahoj");
    check_sv("   a h o j   ", "a h o j");
}

#include "frog/utils/ini.hpp"
#include <string>

TEST_CASE("ini")
{
    const char* content = R"INI(
        [section]
            key = val
            hello  =  world

        [other]
            key = val
            escape    =    " escape \" these "
    )INI";

    const char* to_str = R"INI([section]
key = val
hello = world

[other]
key = val
escape = " escape \" these "
)INI";

    using namespace std::literals;

    auto ini = frog::ini_file(content);
    const auto& const_ini = ini;
    REQUIRE_EQ(ini.at("key"), "val"s);
    REQUIRE_EQ(ini.at("hello"), "world"s);
    REQUIRE_EQ(ini.at("escape"), " escape \" these "s);
    REQUIRE_EQ(const_ini.section_at("section").name, "section"s);
    REQUIRE_EQ(const_ini.section_at("other").name, "other"s);

    REQUIRE_EQ(std::string(ini), std::string(to_str));
}
