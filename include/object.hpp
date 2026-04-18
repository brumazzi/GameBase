#ifndef __OBJECT_HPP__
#define __OBJECT_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <animation.hpp>

namespace game{
    class Object: public std::enable_shared_from_this<Object>{
        public:
        enum class Type{
            // 1 - 99 is diferent objects
            // 100, 200, 300, ... are boss types
            // 101 to N is type of monsters
            PLAYER = 0,

            OBJECT = 1,

            HABBIT_MONSTER = 101
        };
        typedef bool(*Event)(game::Object* object, void* data);
        typedef std::shared_ptr<game::Object> Ptr;

        static Object::Ptr create();

        Object();
        // Object(sf::Texture *texture);
        // Object(std::string texture);
        ~Object();
        sf::Sprite &sprite();
        void update();
        void destroy();
        bool isDestroyed();
        bool isDrawable();
        bool isGrounded();

        void jump();

        // AnimationMap animations();
        game::Animation::Ptr createAnimation(std::string key, std::pair<std::string, sf::Texture*> texture);
        void removeAnimation(std::string key);
        void setAnimation(std::string key);
        std::pair<std::string, game::Animation::Ptr> getAnimation();

        void setDrawable(bool drawable);
        // void setDelay(uint16_t delay);
        void setBodyName(std::string bodyName);
        void setWorldName(std::string worldName);
        void setSize(sf::Vector2f size);
        void setType(game::Object::Type type);
        void setForce(float force);
        void setGrounded(bool grounded = true);
        void setFriction(float friction);

        // uint16_t getDelay();
        std::string getBodyName();
        std::string getWorldName();
        std::string getTextureName();
        sf::Vector2f getSize();
        game::Object::Type getType();
        float getForce();
        float getFriction();

        operator sf::Sprite();

        private:
        std::string m_bodyName;
        std::string m_worldName;
        std::string m_texture;
        sf::Vector2f m_size;
        float m_force;
        float m_friction;
        std::pair<std::string, game::Animation::Ptr> m_animation;
        std::unordered_map<std::string, game::Object::Event> m_callbacks;
        std::unordered_map<std::string, game::Animation::Ptr> m_animations;
        bool m_destroyed;
        bool m_drawable;
        bool m_grounded;
        game::Object::Type m_type;
    };
}

#endif