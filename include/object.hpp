#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <memory>

namespace game{
    typedef std::map<std::string, std::vector<sf::IntRect>> AnimationMap;
    class Object: public std::enable_shared_from_this<Object>{
        public:
        typedef std::shared_ptr<game::Object> Ptr;
        static Object::Ptr create();
        static Object::Ptr create(std::string texture);

        Object();
        Object(sf::Texture *texture);
        Object(std::string texture);
        ~Object();
        sf::Sprite &sprite();
        void update();
        void destroy();
        bool isDestroyed();
        bool isDrawable();

        AnimationMap animations();
        void addAnimation(std::string key, sf::IntRect rect);
        void removeAnimation(std::string key);
        void removeAnimationFrame(std::string key, unsigned int frame);
        void setAnimation(std::string key);

        void setDelay(uint16_t delay);
        void setBodyName(std::string bodyName);
        void setWorldName(std::string worldName);
        void setSize(sf::Vector2f size);

        uint16_t getDelay();
        std::string getBodyName();
        std::string getWorldName();
        std::string getTextureName();
        std::string getAnimation();
        sf::Vector2f getSize();

        operator sf::Sprite();

        private:
        std::string m_bodyName;
        std::string m_worldName;
        std::shared_ptr<sf::Sprite> m_sprite;
        std::string m_texture;
        std::string m_animation;
        sf::Vector2f m_size;
        AnimationMap m_animations;
        uint16_t m_animationDelay;
        uint16_t m_animationDelayCur;
        uint16_t m_animationIndex;
        bool m_destroyed;
        bool m_drawable;
    };
}

#endif