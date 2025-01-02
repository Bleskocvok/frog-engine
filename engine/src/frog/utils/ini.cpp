#include "ini.hpp"

#include "string.hpp"

#include <utility>      // move
#include <sstream>      // ostringstream
#include <cstddef>      // size_t

using namespace frog;

void ini_file::add_section(std::string name)
{
    auto[it, ins] = sec_map.emplace(name, sections_.size());
    if (!ins)
    {
        // TODO: Section already present.
    }
    sections_.push_back(ini_section{ .name = std::move(name), .values = {} });
}

void ini_file::add_value(std::string key, std::string val)
{
    if (sections_.empty())
    {
        // TODO: What to do?
    }
    else
    {
        key_val_map.emplace(key, idx{ .sec = sections_.size() - 1,
                                      .val = sections_.back().values.size() });
        sections_.back().values.emplace_back(std::move(key), std::move(val));
    }
}

static std::string unescaped(std::string_view str)
{
    if (str.size() < 2 || str.front() != '"' || str.back() != '"')
        return std::string(str);

    // TODO: Consider how to properly handle:
    //     key = "value"garbage

    auto out = std::ostringstream{};

    str.remove_prefix(1);
    str.remove_suffix(1);

    int escaped = 0;

    for (char c : str)
    {
        if (c == '\\')
        {
            if (++escaped == 2)
            {
                out << "\\";
                escaped = 0;
            }
        }
        else
        {
            out << c;
            escaped = 0;
        }
    }

    return std::move(out).str();
}

void ini_file::parse(std::string_view view)
{
    constexpr char equals = '=';
    constexpr char begin_section = '[';
    constexpr char end_section = ']';

    auto for_line = [&](std::string_view line)
    {
        trim(line);

        if (line.empty())
            return;

        if (line.front() == begin_section)
        {
            auto end = line.find(end_section);
            if (end == line.npos)
            {
                // TODO: Error.
            }

            auto sec_name = std::string(line.substr(1, end - 1));
            add_section(std::move(sec_name));
            return;
        }

        auto del = line.find(equals);

        if (del == std::string_view::npos)
        {
            // TODO: Error.
        }
        else
        {
            // TODO: Should it be this way?
            if (sections_.empty())
                sections_.push_back(ini_section{ .name = "", .values = {} });

            auto key = line.substr(0, del);
            auto val = del + 1 >= line.size() ? std::string_view("")
                                              : line.substr(del + 1);
            trim(key);
            trim(val);
            add_value(std::string(key), unescaped(val));
        }
    };
    for_each_segment(view, "\n", for_line);
}

static std::string escaped(std::string_view str)
{
    auto out = std::ostringstream{};

    if (str.find('"') != str.npos)
    {
        out << "\"";
        for (char c : str)
        {
            if (c == '"')
                out << '\\' << c;
            else
                out << c;
        }
        out << "\"";
    }
    else
        out << str;

    return std::move(out).str();
}

std::string ini_file::str() const
{
    auto out = std::ostringstream{};

    for (std::size_t i = 0; i < sections_.size(); i++)
    {
        const auto& sec = sections_[i];
        out << "[" << sec.name << "]" << "\n";

        for (const auto&[ k, v ] : sec.values)
            // TODO: Quote and escape appropriately.
            out << k << " = " << escaped(v) << "\n";

        if (i != sections_.size() - 1)
            out << "\n";
    }

    return std::move(out).str();
}

using std::string;

const ini_section& ini_file::section_at(const string& name) const { return section_at_impl(*this, name); }
      ini_section& ini_file::section_at(const string& name)       { return section_at_impl(*this, name); }

const string& ini_file::at(const string& key) const { return at_impl(*this, key); }
      string& ini_file::at(const string& key)       { return at_impl(*this, key); }

const string& ini_file::at(const string& section, const string& key) const { return at_sec_impl(*this, section, key); }
      string& ini_file::at(const string& section, const string& key)       { return at_sec_impl(*this, section, key); }
