#include "string.hpp"

#include <cctype>       // isspace

std::vector<std::string> frog::split(std::string_view str, std::string_view delim)
{
    std::vector<std::string> result;

    auto func = [&](std::string_view segment){ result.emplace_back(segment); };

    for_each_segment(str, delim, func);

    return result;
}

void frog::trim(std::string_view& str)
{
    auto space = [](unsigned char c){ return std::isspace(c); };

    while (not str.empty() && space(str.back()))
        str.remove_suffix(1);

    while (not str.empty() && space(str.front()))
        str.remove_prefix(1);
}

void frog::trim(std::string& str)
{
    auto space = [](unsigned char c){ return std::isspace(c); };

    while (not str.empty() && space(str.back()))
        str.pop_back();

    auto it = str.begin();
    for (auto end = str.end(); it != end && space(*it); it++) ;

    str.erase(str.begin(), it);
}

std::string_view frog::next_segment(std::string_view& str, std::string_view delim)
{
    std::size_t found = str.find(delim);
    if (found != str.npos)
    {
        auto res = str.substr(0, found);
        str.remove_prefix(res.size() + delim.size());
        return res;
    }
    auto res = str.substr();
    str.remove_prefix(res.size());
    return res;
}

std::string_view frog::between(std::string_view view, char open, char close)
{
    auto start = view.find(open);
    auto end = view.find(close);
    if (start == view.npos)
        return std::string_view{};

    view = view.substr(start, end - start);
    if (not view.empty())
        view.remove_prefix(1);

    return view;
}
