#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <object.hpp>
#include <physic.hpp>
#include <SFML/Graphics.hpp>
#include <any>
#include <map>

namespace game{
    class Scene: public std::enable_shared_from_this<Scene>{
        public:
        typedef std::shared_ptr<game::Scene> Ptr;
        static Scene::Ptr create(std::string world);

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

        const std::map<std::string, game::Object::Ptr> objects();
        void objectList(std::vector<std::string>& list);
        bool addObject(game::Object::Ptr object, std::string name = "");
        game::Object::Ptr createObject(std::string texture, std::string name, sf::Vector2f pos, sf::Vector2f size, b2BodyType type = b2_staticBody);

        void removeObject(std::string object);
        void removeObject(game::Object::Ptr object);

        game::Object::Ptr getObject(std::string object);

        // scene vars are local vars, these vars are reseted when scene is reloaded
        void varList(std::vector<std::string>& vector);
        std::string varType(std::string key);
        template<typename T> bool varIs(std::string key);
        template <typename T> T getVar(std::string key);
        template <typename T> void setVar(std::string key, T value);

        private:
        std::string m_name;
        std::string m_world;
        std::map<std::string, game::Object::Ptr> m_objects;
        bool m_showPhysic;
        std::map<std::string, std::any> m_vars;
    };
}

#endif