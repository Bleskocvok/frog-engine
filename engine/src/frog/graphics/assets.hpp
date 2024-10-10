#pragma once

#include "frog/utils/ptr.hpp"

#include <unordered_map>
#include <string>
#include <utility>          // move
#include <stdexcept>        // std::out_of_range, runtime_error


namespace frog::gx
{


template <typename T>
class assets
{
    std::unordered_map<std::string, ptr<T>> data;

    template<typename This>
    static auto& try_get(This* t, const std::string& tag)
    {
        try
        {
            return *t->data.at(tag);
        }
        catch (std::out_of_range&)
        {
            throw std::runtime_error("assets '" + tag + "' not found");
        }
    }

public:
    // TODO: investigate why this segfaults
    // EDIT: apparently it doesn't, but keep an eye on this
    const std::string& add(const std::string& tag, T element)
    {
        return add(tag, mk_ptr<T>(std::move(element)));
    }

    const std::string& add(const std::string& tag, ptr<T> element)
    {
        auto[it, _] = data.emplace(tag, std::move(element));
        return it->first;
    }

    bool remove(const std::string& tag)
    {
        return data.erase(tag) > 0;
    }

    bool contains(const std::string& tag) const
    {
        return data.count(tag);
    }

    // TODO: shove some [[likely]] in there
    T* find(const std::string& tag)
    {
        auto it = data.find(tag);
        if (it == data.end()) [[unlikely]]
            return nullptr;
        else [[likely]]
            return it->second.get();
        // return it == data.end() ? nullptr : it->second.get();
    }

    const T* find(const std::string& tag) const
    {
        auto it = data.find(tag);
        return it == data.end() ? nullptr : it->second.get();
    }

    const T& at(const std::string& tag) const { return try_get(this, tag); }
          T& at(const std::string& tag)       { return try_get(this, tag); }
};


} // namespace frog::gx

