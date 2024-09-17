#pragma once


namespace frog::os {


class window_base
{
public:
    virtual ~window_base() = default;

    virtual bool should_close() const = 0;

    virtual void swap_buffers() = 0;
};


} // namespace frog::os
