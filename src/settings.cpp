#include <settings.hpp>
#include <filesystem>
#include <buttons.hpp>
#include <utils.hpp>

#include <fstream>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

static YAML::Node SETTINGS_NODE;
#define set SETTINGS_NODE

namespace game{
    namespace settings{

        static YAML::Node settingsFindRecursive(const YAML::Node &node, const std::vector<std::string> &path, const uint64_t index){
            for(auto it: node){
                if(it.first.as<std::string>() == path[index]){
                    if(index >= path.size()-1) return it.second;
                    else return settingsFindRecursive(it.second, path, index+1);
                }
            }

            YAML::Node nullNode;
            return nullNode;
        }

        void init(std::string confFile){
            if(std::filesystem::exists(confFile) && std::filesystem::is_regular_file(confFile)){
                SETTINGS_NODE = YAML::LoadFile(confFile);

                game::keyboard::buttons::CONFIRM        = (sf::Keyboard::Key) set["config"]["input"]["keyboard"]["CONFIRM"].as<int>();
                game::keyboard::buttons::MENU           = (sf::Keyboard::Key) set["config"]["input"]["keyboard"]["MENU"].as<int>();
                game::keyboard::buttons::ACTION         = (sf::Keyboard::Key) set["config"]["input"]["keyboard"]["ACTION"].as<int>();
                game::keyboard::buttons::CANCEL         = (sf::Keyboard::Key) set["config"]["input"]["keyboard"]["CANCEL"].as<int>();
                game::keyboard::buttons::USE            = (sf::Keyboard::Key) set["config"]["input"]["keyboard"]["USE"].as<int>();
                game::keyboard::buttons::UP             = (sf::Keyboard::Key) set["config"]["input"]["keyboard"]["UP"].as<int>();
                game::keyboard::buttons::DOWN           = (sf::Keyboard::Key) set["config"]["input"]["keyboard"]["DOWN"].as<int>();
                game::keyboard::buttons::LEFT           = (sf::Keyboard::Key) set["config"]["input"]["keyboard"]["LEFT"].as<int>();
                game::keyboard::buttons::RIGHT          = (sf::Keyboard::Key) set["config"]["input"]["keyboard"]["RIGHT"].as<int>();

                game::joystick::buttons::CONFIRM        = (uint32_t) set["config"]["input"]["joystick"]["CONFIRM"].as<int>();
                game::joystick::buttons::MENU           = (uint32_t) set["config"]["input"]["joystick"]["MENU"].as<int>();
                game::joystick::buttons::ACTION         = (uint32_t) set["config"]["input"]["joystick"]["ACTION"].as<int>();
                game::joystick::buttons::CANCEL         = (uint32_t) set["config"]["input"]["joystick"]["CANCEL"].as<int>();
                game::joystick::buttons::USE            = (uint32_t) set["config"]["input"]["joystick"]["USE"].as<int>();
                game::joystick::buttons::UP             = (uint32_t) set["config"]["input"]["joystick"]["UP"].as<int>();
                game::joystick::buttons::DOWN           = (uint32_t) set["config"]["input"]["joystick"]["DOWN"].as<int>();
                game::joystick::buttons::LEFT           = (uint32_t) set["config"]["input"]["joystick"]["LEFT"].as<int>();
                game::joystick::buttons::RIGHT          = (uint32_t) set["config"]["input"]["joystick"]["RIGHT"].as<int>();
            }else{
                set["config"]["lang"]                           = "en-US";
                set["config"]["style"]["font"]                  = "./resources/fonts/Courier Prime Bold.ttf";
                set["config"]["style"]["window"]["width"]       = WINDOW_WIDTH;
                set["config"]["style"]["window"]["height"]      = WINDOW_HEIGHT;
                set["config"]["style"]["window"]["fullscreen"]  = false;
                set["config"]["game"]["vsync"]                  = false;

                set["config"]["input"]["keyboard"]["CONFIRM"]   = (int32_t) game::keyboard::buttons::CONFIRM;
                set["config"]["input"]["keyboard"]["MENU"]      = (int32_t) game::keyboard::buttons::MENU;
                set["config"]["input"]["keyboard"]["ACTION"]    = (int32_t) game::keyboard::buttons::ACTION;
                set["config"]["input"]["keyboard"]["CANCEL"]    = (int32_t) game::keyboard::buttons::CANCEL;
                set["config"]["input"]["keyboard"]["USE"]       = (int32_t) game::keyboard::buttons::USE;
                set["config"]["input"]["keyboard"]["UP"]        = (int32_t) game::keyboard::buttons::UP;
                set["config"]["input"]["keyboard"]["DOWN"]      = (int32_t) game::keyboard::buttons::DOWN;
                set["config"]["input"]["keyboard"]["LEFT"]      = (int32_t) game::keyboard::buttons::LEFT;
                set["config"]["input"]["keyboard"]["RIGHT"]     = (int32_t) game::keyboard::buttons::RIGHT;

                set["config"]["input"]["joystick"]["CONFIRM"]   = (int32_t) game::joystick::buttons::CONFIRM;
                set["config"]["input"]["joystick"]["MENU"]      = (int32_t) game::joystick::buttons::MENU;
                set["config"]["input"]["joystick"]["ACTION"]    = (int32_t) game::joystick::buttons::ACTION;
                set["config"]["input"]["joystick"]["CANCEL"]    = (int32_t) game::joystick::buttons::CANCEL;
                set["config"]["input"]["joystick"]["USE"]       = (int32_t) game::joystick::buttons::USE;
                set["config"]["input"]["joystick"]["UP"]        = (int32_t) game::joystick::buttons::UP;
                set["config"]["input"]["joystick"]["DOWN"]      = (int32_t) game::joystick::buttons::DOWN;
                set["config"]["input"]["joystick"]["LEFT"]      = (int32_t) game::joystick::buttons::LEFT;
                set["config"]["input"]["joystick"]["RIGHT"]     = (int32_t) game::joystick::buttons::RIGHT;

                set["config"]["paths"]["translates"]            = "./resources/translates/";
                // TODO: configure this to suport any paths to music, sound and texture
                // add correct on resources load to suport load diferent paths
                set["config"]["paths"]["resources"].push_back("./resources/musics/");
                set["config"]["paths"]["resources"].push_back("./resources/sounds/");
                set["config"]["paths"]["resources"].push_back("./resources/textures/");
            }
        }
        bool save(std::string confFile){
            std::ofstream outFile(confFile);
            if(outFile.is_open()){
                outFile << SETTINGS_NODE;
                outFile.close();
                return true;
            }

            return false;
        }
        template<typename T> T getProperty(std::string prop){
            std::vector<std::string> splitedProp;
            game::string::split(prop, splitedProp, '.');
            YAML::Node node = settingsFindRecursive(SETTINGS_NODE, splitedProp, 0);
            return node.as<T>();
        }
        YAML::Node getPropertyNode(std::string prop){
            std::vector<std::string> splitedProp;
            game::string::split(prop, splitedProp, '.');

            return settingsFindRecursive(SETTINGS_NODE, splitedProp, 0);
        }

        YAML::Node& getSettings(){
            return SETTINGS_NODE;
        }
        template<typename T> void setProperty(std::string prop, T value){
            std::vector<std::string> splitedProp;
            game::string::split(prop, splitedProp, '.');

            YAML::Node buff = settingsFindRecursive(SETTINGS_NODE, splitedProp, 0);
            buff = value;
        }

        std::string getHomeDir() {
        #ifdef _WIN32
            // Windows: Obtém a pasta AppData
            char path[MAX_PATH];
            if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
                return std::string(path);
            } else {
                return "";
            }
        #else
            // Linux: Obtém a pasta home do usuário
            struct passwd *pw = getpwuid(getuid());
            if (pw && pw->pw_dir) {
                return std::string(pw->pw_dir);
            } else {
                return "";
            }
        #endif
        }

        template std::string getProperty<std::string>(std::string);
        template int getProperty<int>(std::string);
        template unsigned int getProperty<unsigned int>(std::string);
        template double getProperty<double>(std::string);
        template bool getProperty<bool>(std::string);

        template void setProperty<std::string>(std::string, std::string);
        template void setProperty<int>(std::string, int);
        template void setProperty<unsigned int>(std::string, unsigned int);
        template void setProperty<double>(std::string, double);
        template void setProperty<bool>(std::string, bool);
    }
}