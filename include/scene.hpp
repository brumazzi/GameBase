#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <object.hpp>
#include <physic.hpp>
#include <SFML/Graphics.hpp>
#include <any>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

namespace game{
    namespace scene{
        enum Layer{
            BACKGROUND      = 0,
            FAR_BACKGROUND  = 1,
            MID_BACKGROUND  = 2,
            NEAR_BACKGROUND = 3,
            OBJECT_FIELD    = 4,
            NEAR_FOREGROUND = 5,
            MID_FOREGROUND  = 6,
            FAR_FOREGROUND  = 7,
            HUB             = 8,
            LEVEL_COUNT     = 9
        };
    }
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
        void setGrid(sf::Vector2u grid);
        std::string getWorld();
        bool getShowPhysic();
        std::string getName();
        sf::Vector2u getGrid();

        const std::unordered_map<std::string, game::Object::Ptr> objects();
        void objectList(std::vector<std::string>& list);
        bool addObject(game::Object::Ptr object, std::string name = "");
        game::Object::Ptr createObject(std::string texture, std::string name, sf::Vector2f pos, sf::Vector2f size, b2BodyType type = b2_staticBody);
        void addSprite(game::scene::Layer layerID, std::string texture, sf::IntRect rect, sf::Vector2f pos);
        void addCollisionArea(std::string name, sf::FloatRect rect);

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
        sf::Vector2u m_grid;
        std::unordered_map<std::string, game::Object::Ptr> m_objects;
        std::unordered_map<std::string, std::any> m_vars;
        std::map<
            game::scene::Layer, std::unordered_map<
                long long int, std::unordered_map<
                    long long int, sf::Sprite*
                >
            >
        > m_layers;
        bool m_showPhysic;
    };
}

#endif