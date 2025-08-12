#include <utils.hpp>
#include <unistr.h>
#include <string>
#include <iostream>
#include <filesystem>

namespace game{
    namespace string{
        std::u32string str_to_utf32(std::string str) {
            std::u32string result;

            size_t size;
            uint32_t* utf32 = u8_to_u32(reinterpret_cast<const uint8_t*>(str.data()), str.size(), nullptr, &size);
            if(!utf32) return U"";

            result.assign(utf32, utf32 + size);
            free(utf32);
            return result;
        }

        std::string toLowercase(std::string str) {
            std::transform(str.begin(), str.end(), str.begin(),
                        [](unsigned char c) { return std::tolower(c); });
            return str;
        }

        void split(const std::string& str, std::vector<std::string> &output, char delimiter) {
            std::stringstream ss(str);
            std::string token;

            while (std::getline(ss, token, delimiter)) {
                output.push_back(token);
            }
        }
    }
}