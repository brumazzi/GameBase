#include <settings.hpp>
#include <filesystem>
#include <utils.hpp>

static YAML::Node Translates;

#define if_translate(label) if(label.size() > 2 && label[0] == 't' && label[1] == ':')

namespace game{
    namespace translate{

        static YAML::Node translatesFindRecursive(const YAML::Node &node, const std::vector<std::string> &path, const uint64_t index){
            for(auto it: node){
                if(!it.first.as<std::string>().compare(path[index])){
                    if(index == path.size()-1){
                        return it.second;
                    }
                    else return translatesFindRecursive(it.second, path, index+1);
                }
            }

            YAML::Node nullNode;
            return nullNode;
        }

        void load(){
            for(auto it: std::filesystem::directory_iterator(::game::settings::getProperty<std::string>("config.paths.translates"))){
                if(it.path().extension().string() == ".yaml" || it.path().extension().string() == ".yml"){
                    YAML::Node node = YAML::LoadFile(it.path().string());
                    std::string name = it.path().filename().string();

                    uint8_t extSize = (it.path().extension().string() == ".yaml") ? 5 : 4;
                    std::string lang = name.replace(name.end()-extSize, name.end(), "");

                    Translates[lang] = node[lang];
                }
            }
        }
        std::string get(std::string path){
            std::vector<std::string> splitedPath;
            game::string::split(path, splitedPath, '.');
            YAML::Node node = translatesFindRecursive(Translates[::game::settings::getProperty<std::string>("config.lang")], splitedPath, 0);
            if(node.IsScalar()) return node.as<std::string>();

            return path;
        }
        void setLang(std::string lang){
            ::game::settings::getSettings()["config"]["lang"] = lang;
        }
    }
}