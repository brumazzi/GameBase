#ifndef __VARS_HPP__
#define __VARS_HPP__

#include <iostream>
#include <vector>

namespace game{
    namespace vars{
        void list(std::vector<std::string>& vector);
        std::string type(std::string key);
        template<typename T> bool is(std::string key);
        template <typename T> T get(std::string key);
        template <typename T> bool set(std::string key, T value);
    }
}

#endif