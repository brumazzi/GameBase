#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

namespace game{
    class Object{
        public:
        Object();
        Object(sf::Texture *texture);
        Object(std::string texture);
        ~Object();
        sf::Sprite &sprite();
        void update();
        void destroy();
        bool isDestroyed();
        bool isDrawable();

        void addAnimation(std::string key, sf::IntRect rect);
        void removeAnimation(std::string key);
        void removeAnimationFrame(std::string key, unsigned int frame);
        void setAnimation(std::string key);
        std::string getAnimation();

        void setDelay(uint16_t delay);
        void setBodyName(std::string bodyName);
        void setWorldName(std::string worldName);
        uint16_t getDelay();
        std::string getBodyName();
        std::string getWorldName();

        operator sf::Sprite();

        private:
        std::string m_bodyName;
        std::string m_worldName;
        sf::Sprite *m_sprite;
        std::string m_animation;
        std::map<std::string, std::vector<sf::IntRect>> m_animations;
        uint16_t m_animationDelay;
        uint16_t m_animationDelayCur;
        uint16_t m_animationIndex;
        bool m_destroyed;
        bool m_drawable;
    };
}

#endif