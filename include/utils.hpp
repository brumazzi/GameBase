#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <iostream>
#include <string>
#include <algorithm>
#include <string>
#include <unistr.h>
#include <uniconv.h>
#include <vector>
#include <thread>

namespace game{
    namespace string{
        std::u32string str_to_utf32(std::string str);
        std::string toLowercase(std::string str);
        void split(const std::string& str, std::vector<std::string> &output, char delimiter);
    }

    namespace async{
    }
}

#endif