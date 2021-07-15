#pragma once

#include "utils/ptr.hpp"

#include <unordered_map>
#include <string>
#include <utility>          // move


namespace frog::gl
{


template <typename T>
class assets
{
    std::unordered_map<std::string, ptr<T>> data;

public:
    // TODO investigate why this segfaults
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

    const T& at(const std::string& tag) const { return *data.at(tag); }
          T& at(const std::string& tag)       { return *data.at(tag); }
};


} // namespace frog

