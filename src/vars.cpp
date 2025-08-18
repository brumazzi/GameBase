#include <vars.hpp>
#include <map>
#include <any>

std::map<std::string, std::any> VAR_NODE;

namespace game{
    namespace vars{
        void list(std::vector<std::string>& vector){
            for(auto it: VAR_NODE){
                vector.emplace_back(it.first);
            }
        }
        std::string type(std::string key){
            return std::string(VAR_NODE[key].type().name());
        }
        template<typename T> bool is(std::string key){
            return VAR_NODE[key].type() == typeid(T);
        }

        template <typename T> T get(std::string key){
            return std::any_cast<T>(VAR_NODE[key]);
        }
        template <typename T> bool set(std::string key, T value){
            VAR_NODE[key] = value;
            return std::any_cast<T>(VAR_NODE[key]) == value;
        }

        template std::string get<std::string>(std::string key);
        template long get<long>(std::string key);
        template double get<double>(std::string key);
        template bool get<bool>(std::string key);

        template bool set<std::string>(std::string key, std::string value);
        template bool set<long>(std::string key, long value);
        template bool set<double>(std::string key, double value);
        template bool set<bool>(std::string key, bool value);

        template bool is<std::string>(std::string key);
        template bool is<long>(std::string key);
        template bool is<double>(std::string key);
        template bool is<bool>(std::string key);
    }
}