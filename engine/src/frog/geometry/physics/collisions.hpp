#pragma once

#include "idx.hpp"
#include "primitives.hpp"
#include "container.hpp"

#include "frog/debug.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <set>
#include <unordered_set>
#include <utility>          // move, pair, forward
#include <cstddef>          // size_t
#include <algorithm>        // remove_if

namespace frog::geo {

using CollisionInfo = std::pair<idx_t, idx_t>;

} // namespace frog::geo

namespace std {

template <>
struct hash< frog::geo::CollisionInfo >
{
    size_t operator()(const frog::geo::CollisionInfo& info) const
    {
        auto to_uint = [](frog::geo::idx_t x)
        {
            bool neg1 = x < 0;
            std::uint64_t val = std::abs(x);
            val |= std::uint64_t(neg1) << 31;
            return val;
        };

        std::uint64_t a = to_uint(info.first);
        std::uint64_t b = to_uint(info.second);

        std::uint64_t comb = a | ( b << 32 );
        return std::hash<decltype(comb)>{}(comb);
    }
};

} // namespace std

namespace frog::geo {

template<class T>
struct CollisionBag
{
    std::unordered_set<CollisionInfo> data;
    std::unordered_set<idx_t> bag;

    void clear()
    {
        data.clear();
        bag.clear();
    }

    bool contains(CollisionInfo info) const
    {
        return data.contains(info);
    }

    auto begin()       { return data.begin(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
    auto end()       { return data.end(); }

    auto insert(CollisionInfo info)
    {
        auto ret = data.insert(info);
        bag.insert(info.first);
        bag.insert(info.second);
        return ret;
    }

    template<class F>
    friend size_t erase_if(CollisionBag& cont, F&& f)
    {
        auto ret = std::erase_if(cont.data, std::forward<F>(f));

        cont.bag.clear();

        for (const auto&[a, b] : cont.data)
        {
            cont.bag.insert(a);
            cont.bag.insert(b);
        }

        return ret;
    }
};

namespace detail {

struct ContainsInfo
{
    static inline std::array<std::uint64_t, 3> COUNTS = { 0 };

    static auto& bag_count()
    {
        return COUNTS[ 0 ];
    }

    static auto& set_count()
    {
        return COUNTS[ 1 ];
    }

    static auto& unordered_set_count()
    {
        return COUNTS[ 2 ];
    }

    static void reset()
    {
        COUNTS = { 0 };
    }
};

inline bool contains(const CollisionBag<CollisionInfo>& container, idx_t i)
{
    ContainsInfo::bag_count()++;

    return container.bag.contains(i);
}

inline bool contains(const std::set<CollisionInfo>& container, idx_t i)
{
    ContainsInfo::set_count()++;

    for (const auto& info : container)
    {
        if (info.first == i || info.second == i)
            return true;
    }
    return false;
}

inline bool contains(const std::unordered_set<CollisionInfo>& container, idx_t i)
{
    ContainsInfo::unordered_set_count()++;

    for (const auto& info : container)
    {
        if (info.first == i || info.second == i)
            return true;
    }
    return false;
}

} // namespace detail

template<template<typename... Args> typename CollisionContainer>
struct Collisions
{
    static constexpr idx_t BOUNDS = -1;

    CollisionContainer<CollisionInfo> current_;
    CollisionContainer<CollisionInfo> first_;
    CollisionContainer<CollisionInfo> all_;
    CollisionContainer<CollisionInfo> all_without_joints_;

    void reset()
    {
        current_.clear();
        first_.clear();
    }

    void update(const Container<Joint>& joints)
    {
        using std::erase_if;
        erase_if(all_, [&](const auto& info)
        {
            return not current_.contains(info);
        });
        erase_if(all_without_joints_, [&](const auto& info)
        {
            return not current_.contains(info);
        });

        auto always = [](const auto& info)
        {
            return info.first == BOUNDS || info.second == BOUNDS;
        };

        for (const auto& info : first_)
        {
            all_.insert(info);

            auto joint_collision = [&](const auto& pair) -> bool
            {
                const Joint& j = pair.second;

                return ( j.a == info.first  && j.b == info.second )
                    || ( j.a == info.second && j.b == info.first );
            };

            if (always(info)
                    || not std::ranges::any_of(joints.data, joint_collision))
                all_without_joints_.insert(info);
        }
    }

    void insert_collision(CollisionInfo info)
    {
        current_.insert(info);
        if (not all_.contains(info))
            first_.insert(info);
    }

    const auto& first()   const { return first_; }
    const auto& current() const { return current_; }
    const auto& all()     const { return all_; }
    const auto& all_without_joints() const { return all_without_joints_; }

    bool contains(const CollisionContainer<CollisionInfo>& container, idx_t i) const
    {
        return detail::contains(container, i);

        // if constexpr (std::is_same_v<CollisionContainer<CollisionInfo>, std::set<CollisionInfo>>)
        // {
        //     container.dasdads();
        //     for (const auto& info : container)
        //     {
        //         if (info.first == i || info.second == i)
        //             return true;
        //     }
        //     return false;
        // }
        // else
        // {
        //     return container.bag.contains(i);
        // }

        // // TODO: Perhaps some unit tests?
        // auto lower = container.lower_bound({ i,     0 });
        // auto upper = container.lower_bound({ i + 1, 0 });
        // if (lower != upper)
        //     return true;

        // auto end = container.upper_bound({ i, 0 });
        // for (auto it = container.begin(); it != end; ++it)
        // {
        //     const auto& [a, b] = *it;
        //     if (b == i)
        //         return true;
        // }
        // return false;
    }
};

} // namespace frog::geo
