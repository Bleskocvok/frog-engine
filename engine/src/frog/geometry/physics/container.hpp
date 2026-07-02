#pragma once

#include "idx.hpp"

#include <utility>          // move, pair, forward
#include <cstddef>          // size_t
#include <stdexcept>        // runtime_error
#include <vector>           // vector
#include <algorithm>        // remove_if

namespace frog::geo {

// All of this to allow for deletion, because index reuse becomes quite
// necessary.
template<typename T>
struct Container
{
    idx_t limit_ = 0;
    std::vector<std::pair<idx_t, T>> data;
    std::vector<idx_t> free;
    std::vector<int> map;

    explicit Container(int limit) : limit_(limit), map(limit, -1)
    {
        data.reserve(limit);
    }

    template<typename... Args>
    idx_t emplace(Args&&... args)
    {
        if (free.empty() && limit_ >= 0 && size() >= static_cast<size_t>(limit_))
            throw std::runtime_error("physics container emplace: limit reached");

        idx_t i;

        if (not free.empty())
        {
            i = free.back();
            free.pop_back();
        }
        else
            i = data.size();

        data.emplace_back(i, T( std::forward<Args>(args)... ));
        map[i] = data.size() - 1;
        return i;
    }

    idx_t push(T val)
    {
        return emplace(std::move(val));
    }

    void remove(idx_t i)
    {
        if (i >= limit_)
            throw std::runtime_error("physics container: invalid index");
        if (map[i] == -1)
            throw std::runtime_error("physics container: cannot delete a nonexistent element");

        auto removed = map[i];
        data.erase(data.begin() + removed);
        map[i] = -1;
        free.push_back(i);

        for (auto& idx : map)
        {
            if (idx > removed)
                --idx;
        }
    }

    template<template<typename...> typename Vec>
    void remove(const Vec<idx_t>& vec)
    {
        if (vec.empty())
            return;

        if (vec.size() == 1)
        {
            remove(vec.front());
            return;
        }

        for (auto i : vec)
        {
            if (i >= limit_)
                throw std::runtime_error("physics container: invalid index");
            if (map[i] == -1)
                throw std::runtime_error("physics container: cannot delete a nonexistent element");
        }

        for (auto i : vec)
        {
            if (map[i] == -1)
                continue;
            data[ map[i] ].first = -1;
            map[i] = -1;
            free.push_back(i);
        }

        data.erase(std::remove_if(data.begin(), data.end(),
                   [](const auto& elem){ return elem.first == idx_t(-1); }),
                   data.end());

        for (std::size_t i = 0; i < data.size(); ++i)
            map[ data[i].first ] = i;
    }

    std::size_t size() const { return data.size(); }

    const T& at(idx_t i) const { return data[ map[ i ] ].second; }
          T& at(idx_t i)       { return data[ map[ i ] ].second; }
};

} // namespace frog::geo
