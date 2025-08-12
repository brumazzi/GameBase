#include <vars.hpp>
#include <yaml-cpp/yaml.h>

YAML::Node VAR_NODE;

namespace game{
    namespace vars{
        template <typename T> T get(std::string key){
            return VAR_NODE[key].as<T>();
        }
        template <typename T> bool set(std::string key, T value){
            VAR_NODE[key] = value;
            return VAR_NODE.as<T>() == value;
        }

        template std::string get(std::string key);
        template long get(std::string key);
        template double get(std::string key);
        template bool get(std::string key);


        template bool set(std::string key, std::string value);
        template bool set(std::string key, long value);
        template bool set(std::string key, double value);
        template bool set(std::string key, bool value);
    }
}