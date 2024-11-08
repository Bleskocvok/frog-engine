#pragma once

#include <string>           // string
#include <utility>          // move, pair
#include <string_view>      // string_view
#include <vector>           // vector
#include <unordered_map>    // unordered_map
#include <cstddef>          // size_t

#include "frog/utils/todo.hpp"

namespace frog
{

struct ini_section
{
    std::string name;
    std::vector<std::pair<std::string, std::string>> values;
};

class ini_file
{
    std::vector<ini_section> sections_;

    struct idx { std::size_t sec, val; };
    std::unordered_map<std::string, idx> key_val_map;
    std::unordered_map<std::string, std::size_t> sec_map;

    void add_section(std::string name);
    void add_value(std::string key, std::string val);

    void parse(std::string_view view);

    ini_section& section_at(const std::string& name);

    template<typename T>
    static auto& section_at_impl(T& obj, const std::string& name)
    {
        if (auto it = obj.sec_map.find(name); it != obj.sec_map.end())
            return obj.sections_[it->second];

        throw error("ini::section_at: invalid section '", name, "'");
    }

    template<typename T>
    static auto& at_impl(T& obj, const std::string& key)
    {
        if (auto it = obj.key_val_map.find(key); it != obj.key_val_map.end())
            return obj.sections_[it->second.sec].values[it->second.val].second;

        throw error("ini::at: invalid key '", key, "'");
    }

    template<typename T>
    static auto& at_sec_impl(T& obj, const std::string& sec, const std::string& key)
    {
        // TODO: Optimize this.
        for (auto&[k, v] : obj.section_at(sec).values)
            if (k == key)
                return v;

        throw error("ini::at(sec, key): invalid key '", key, "'");
    }

public:
    explicit ini_file(const std::string& content)
    {
        parse(content);
    }

    operator std::string() const { return str(); }

    std::string str() const;

    const ini_section& section_at(const std::string& name) const;

    const auto& sections() const { return sections_; }

    const std::string& at(const std::string& key) const;
          std::string& at(const std::string& key);

    const std::string& at(const std::string& section, const std::string& key) const;
          std::string& at(const std::string& section, const std::string& key);

    bool remove_section(const std::string& name) { TODO(); }
    bool remove_value(const std::string& key) { TODO(); }
};

} // namespace frog
