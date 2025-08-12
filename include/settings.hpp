#define __VISUAL_NOVEL

#ifndef __SETTINGS_HPP__
#define __SETTINGS_HPP__

#include <SFML/Graphics.hpp>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>

#include <SFGUI/Widget.hpp>

#define PROJECT_NAME "CozyGame"
#define WINDOW_WIDTH 1368
#define WINDOW_HEIGHT 768

namespace game{
    namespace settings{
        void init(std::string confFile);
        bool save(std::string confFile);
        template<typename T> T getProperty(std::string prop);
        YAML::Node getPropertyNode(std::string prop);
        YAML::Node& getSettings();
        template<typename T> void setProperty(std::string prop, T value);

        bool openSettingWindow(sf::RenderWindow &window);
    }
    namespace translate{
        void load();
        void setLang(std::string lang);
        std::string get(std::string path);
        void applyToWidget(sfg::Widget::Ptr widget);
    }
}
#define SETTINGS ::game::settings::getSettings()
#define t(path) ::game::translate::get(path)

#endif