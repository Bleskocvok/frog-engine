#pragma once

#include "frog/utils/ptr.hpp"

#include <algorithm>    // clamp
#include <atomic>
#include <functional>
#include <utility>      // move
#include <thread>       // thread
#include <unordered_map>
#include <string>
#include <vector>

namespace frog {

namespace detail {

struct JobData
{
    std::atomic_bool done = false;
};

inline void runner(JobData* data, std::function<void()> f)
{
    f();
    data->done = true;
}

} // namespace detail

class AsyncJobs
{
    std::unordered_map<std::string, frog::ptr<std::thread>> async_jobs;

    std::unordered_map<std::string, frog::ptr<detail::JobData>> data;

public:
    AsyncJobs() = default;

    ~AsyncJobs()
    {
        for (const auto& [key, val] : data)
            async_jobs.at(key)->join();
    }

    AsyncJobs(const AsyncJobs&) = delete;
    AsyncJobs& operator=(const AsyncJobs&) = delete;

    bool add(std::string name, std::function<void()> f)
    {
        auto [it, added] = data.emplace(name, frog::mk_ptr<detail::JobData>());
        if (not added)
            return false;

        async_jobs.emplace(std::move(name),
                           frog::mk_ptr<std::thread>(detail::runner, it->second.get(), std::move(f)));

        return true;
    }

    bool finished(const std::string& name) const
    {
        auto it = data.find(name);
        if (it == data.end())
            return true;
        return it->second->done;
    }

    void cleanup_finished()
    {
        std::vector<std::string> to_delete;

        for (const auto& [key, val] : data)
        {
            if (finished(key))
            {
                async_jobs.at(key)->join();
                to_delete.push_back(key);
            }
        }

        for (const auto& key : to_delete)
        {
            async_jobs.erase(key);
            data.erase(key);
        }
    }
};

} // namespace frog
