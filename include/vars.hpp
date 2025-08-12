#ifndef __VARS_HPP__
#define __VARS_HPP__

#include <iostream>

namespace game{
    namespace vars{
        template <typename T> T get(std::string key);
        template <typename T> bool set(std::string key, T value);
    }
}

#endif