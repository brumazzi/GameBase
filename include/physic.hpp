#ifndef __PHYSIC_HPP__
#define __PHYSIC_HPP__

#include <iostream>
#include <box2d/box2d.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

namespace game{
    class Object;

    namespace physic{
        namespace world{
            b2WorldId& create(double gravityX, double gravityY, std::string world = "default");
            b2WorldId& create(sf::Vector2f gravity, std::string world = "default");
            b2WorldId& get(std::string world = "default");
            void destroy(std::string world = "default");
            bool exists(std::string world = "default");

            void active(std::string world = "default");
            void desactive(std::string world = "default");

            sf::Vector2f getGravity(std::string world = "default");
            void setGravity(sf::Vector2f gravity, std::string world = "default");

            void update(std::string world = "default");
            void update(std::vector<std::string> worlds);

            void drawLines(sf::RenderWindow& render);
            void drawLines(sf::RenderWindow& render, std::string world = "default");
            void drawLines(sf::RenderWindow& render, std::vector<std::string> worlds);
        }
        namespace body{
            b2BodyId& create(std::string world, std::string bodyName, std::shared_ptr<game::Object> object, sf::Vector2f position, sf::Vector2f size, b2BodyType type = b2_staticBody, float density = 1.0, float friction = 0.0, float restitution = 0.0, bool fixRotation = true);
            b2BodyId& get(std::string world, std::shared_ptr<game::Object> object);
            b2BodyId& get(std::string world, std::string body);

            void destroy(std::string world, std::shared_ptr<game::Object> object);
            void destroy(std::string world, std::string body);
            bool exists(std::string world, std::shared_ptr<game::Object> object);
            bool exists(std::string world, std::string body);

            game::Object* getObject(std::string world, std::string body);
            sf::Vector2f getPosition(std::string world, std::string body);
            sf::Vector2f getVelocity(std::string world, std::string body);
            // sf::Vector2f getSize(std::string world, std::string body);
            b2BodyType getType(std::string world, std::string body);
            float getDensity(std::string world, std::string body);
            float getFriction(std::string world, std::string body);
            float getRestitution(std::string world, std::string body);
            bool getFixRotation(std::string world, std::string body);

            sf::Vector2f getPosition(std::string world, std::shared_ptr<game::Object> object);
            sf::Vector2f getVelocity(std::string world, std::shared_ptr<game::Object> object);
            // sf::Vector2f getSize(std::string world, std::shared_ptr<game::Object> object);
            b2BodyType getType(std::string world, std::shared_ptr<game::Object> object);
            float getDensity(std::string world, std::shared_ptr<game::Object> object);
            float getFriction(std::string world, std::shared_ptr<game::Object> object);
            float getRestitution(std::string world, std::shared_ptr<game::Object> object);
            bool getFixRotation(std::string world, std::shared_ptr<game::Object> object);

            void setPosition(std::string world, std::string body, sf::Vector2f position);
            void setVelocity(std::string world, std::string body, sf::Vector2f velocity);
            // void setSize(std::string world, std::string body, sf::Vector2f size);
            void setType(std::string world, std::string body, b2BodyType type);
            void setDensity(std::string world, std::string body, float density);
            void setFriction(std::string world, std::string body, float friction);
            void setRestitution(std::string world, std::string body, float restitution);
            void setFixRotation(std::string world, std::string body, bool fixRotation);

            void setPosition(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f position);
            void setVelocity(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f velocity);
            // void setSize(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f size);
            void setType(std::string world, std::shared_ptr<game::Object> object, b2BodyType type);
            void setDensity(std::string world, std::shared_ptr<game::Object> object, float density);
            void setFriction(std::string world, std::shared_ptr<game::Object> object, float friction);
            void setRestitution(std::string world, std::shared_ptr<game::Object> object, float restitution);
            void setFixRotation(std::string world, std::shared_ptr<game::Object> object, bool fixRotation);
        }
    }
}

#endif