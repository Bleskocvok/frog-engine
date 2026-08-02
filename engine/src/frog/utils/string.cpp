#include "string.hpp"

#include <cctype>       // isspace
#include <string>

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

std::string_view frog::between(std::string_view view, char open, char close, bool require_start)
{
    auto start = view.find(open);
    auto end = view.find(close);

    bool start_found = start != view.npos;

    if (require_start && not start_found)
        return std::string_view{};

    if (not require_start && not start_found)
        start = 0;

    view = view.substr(start, end - start);
    if (not view.empty() && start_found)
        view.remove_prefix(1);

    return view;
}

std::string frog::cut_lines_copy(std::string s, int max_len, const std::string& next_prefix)
{
    std::string res;
    res.reserve(s.length());

    int len = 0;

    for (auto c : s)
    {
        if (c == '\n')
            len = 0;
        else
            len++;

        res.push_back(c);

        if (len >= max_len)
        {
            res.push_back('\n');
            res.append(next_prefix);
            len = 0;
        }
    }

    return res;
}
