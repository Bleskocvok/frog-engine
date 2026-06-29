#pragma once

#include <algorithm>        // max_element
#include <list>
#include <stdexcept>
#include <unordered_map>
#include <map>
#include <utility>          // move

namespace frog::font {

template<typename Key, typename Val>
struct Cache
{
    struct Entry
    {
        Key key;
        Val val;
        Entry(Key k, Val v)
            : key(std::move(k))
            , val(std::move(v))
        { }
    };

    std::list<Entry> lru;
    using Iterator = decltype(lru)::iterator;
    std::map<Key, Iterator> map;

    unsigned max_size;

    explicit Cache(unsigned size) : max_size(size) {}

    Val& put(Key key, Val val)
    {
        // Precond: get() returns nullptr.

        while (map.size() >= max_size)
            remove_oldest();

        lru.emplace_front(key, std::move(val));
        auto [it, ok] = map.emplace(std::move(key), lru.begin());

        return lru.begin()->val;
    }

    Val* get(const Key& key)
    {
        auto it = map.find(key);
        if (it == map.end())
            return nullptr;

        auto lru_it = it->second;
        lru.splice(lru.begin(), lru, lru_it);

        return &lru_it->val;
    }

    bool is_present(const Key& key) const { return map.contains(key); }

    void remove_oldest()
    {
        if (lru.empty())
            throw std::runtime_error("cache::remove_oldest: empty");

        const auto& k = lru.back().key;
        map.erase(k);

        lru.pop_back();
    }

    auto size() const { return map.size(); }
    auto begin() const { return lru.begin(); }
    auto end() const { return lru.end(); }
};

} // namespace frog::font
