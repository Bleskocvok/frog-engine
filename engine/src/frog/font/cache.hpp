#pragma once

#include <algorithm>        // max_element
#include <unordered_map>
#include <utility>          // move

namespace frog::font {

template<typename Key, typename Val>
struct Cache
{
    struct entry
    {
        int age = 0;
        Val val;
        entry(Val val) : val(std::move(val)) {}
    };

    std::unordered_map<Key, entry> data;
    unsigned max_size;

    explicit Cache(unsigned size) : max_size(size) {}

    Val& put(Key key, Val val)
    {
        while (data.size() >= max_size)
            remove_oldest();

        auto it = data.emplace(std::move(key), std::move(val)).first;
        return it->second.val;
    }

    Val* get(const Key& key)
    {
        auto it = data.find(key);
        if (it == data.end())
            return nullptr;

        it->second.age--;
        return &it->second.val;
    }

    bool is_present(const Key& key) const { return data.contains(key); }

    void remove_oldest()
    {
        auto it = std::max_element(data.begin(), data.end(),
                [](const auto& a, const auto& b)
                {
                    return a.second.age < b.second.age;
                });

        data.erase(it);
    }
};

} // namespace frog::font
