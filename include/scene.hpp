#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <object.hpp>
#include <physic.hpp>
#include <SFML/Graphics.hpp>
#include <yaml-cpp/yaml.h>

namespace game{
    class Scene{
        public:
        Scene(std::string world);
        ~Scene();

        void update();
        void draw(sf::RenderWindow& render);
        void event(std::optional<sf::Event> event);

        void setWorld(std::string world);
        void setShowPhysic(bool flag);
        void setName(std::string name);
        std::string getWorld();
        bool getShowPhysic();
        std::string getName();

        bool addObject(game::Object* object, std::string name = "");
        game::Object* createObject(std::string texture, std::string name, sf::Vector2f pos, sf::Vector2f size, b2BodyType type = b2_staticBody);

        void removeObject(std::string object);
        void removeObject(game::Object* object);

        game::Object* getObject(std::string object);

        // scene vars are local vars, these vars are reseted when scene is reloaded
        template <typename T> T getVar(std::string key);
        template <typename T> void setVar(std::string key, T value);

        private:
        std::string m_name;
        std::string m_world;
        std::map<std::string, game::Object*> m_objects;
        bool m_showPhysic;
        YAML::Node m_vars;
    };
}

#endif