#pragma once

#include "base.hpp"

#include "frog/lib2d/font.hpp"
#include "frog/lib2d/texture.hpp"

#include <algorithm>        // max_element
#include <unordered_map>
#include <utility>          // move


namespace frog::font {


template<typename Key, typename Val>
struct cache
{
    struct entry
    {
        int age = 0;
        Val val;
        entry(Val val) : val(std::move(val)) {}
    };

    std::unordered_map<Key, entry> data;
    unsigned max_size;

    explicit cache(unsigned size) : max_size(size) {}

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


class truetype : public base
{
    lib2d::font font_;

    static constexpr int CACHE_SIZE = 1000;
    cache<std::string, frog::lib2d::gx::texture> texture_cache{ CACHE_SIZE };

public:
    truetype(const std::string& file, int size, int outline = 0);

    frog::geo::vec2 size(const std::string& str, float height) override;

    void draw(frog::engine2d&, const std::string& str,
              frog::geo::vec2 pos, float height, frog::gx::rgba_t color) override;
};


} // namespace frog
