#pragma once

#include "frog/scripts/button/button_action.hpp"
#include "frog/debug.hpp"

#include <string>

namespace frog::scripts {

template<typename Base>
struct log_action_base;

using log_action = log_action_base<button_action>;
using log_action2d = log_action_base<button_action2d>;

template<typename Base>
struct log_action_base : Base
{
    std::string str;

    explicit log_action_base(std::string str)
        : str(std::move(str)) {}

    void action(typename Base::GameObject&, typename Base::Engine& engine) override
    {
        LOG(str);
    }

};

} // namespace frog::scripts
