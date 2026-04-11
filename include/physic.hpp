#ifndef __PHYSIC_HPP__
#define __PHYSIC_HPP__

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
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
            void eventBeginTouch(std::string world = "default");
            void eventEndTouch(std::string world = "default");
            void eventHit(std::string world = "default");

            void drawLines(sf::RenderWindow& render);
            void drawLines(sf::RenderWindow& render, std::string world = "default");
            void drawLines(sf::RenderWindow& render, std::vector<std::string> worlds);
        }
        namespace body{
            enum ObjectFilterType{
                NO_BODY         = 0,
                GROUND          = 1,
                SOLID           = 1 << 1,
                SCENE_OBJECT    = 1 << 2,
                GHOST           = 1 << 3,
                ATTACK          = 1 << 4,
                GHOST_ATTACK    = 1 << 5
            };

            enum ObjectFilterTypeMask{
                NO_BODY_MASK        = 0,
                GROUND_MASK         = ObjectFilterType::SOLID | ObjectFilterType::SCENE_OBJECT | ObjectFilterType::ATTACK,
                SOLID_MASK          = ObjectFilterType::SOLID | ObjectFilterType::ATTACK | ObjectFilterType::GHOST_ATTACK | ObjectFilterType::GROUND,
                SCENE_OBJECT_MASK   = ObjectFilterType::ATTACK | ObjectFilterType::GHOST_ATTACK | ObjectFilterType::GROUND,
                GHOST_MASK          = ObjectFilterType::ATTACK | ObjectFilterType::GHOST_ATTACK,
                ATTACK_MASK         = ObjectFilterType::SOLID | ObjectFilterType::SCENE_OBJECT | ObjectFilterType::GROUND,
                GHOST_ATTACK_MASK   = ObjectFilterType::SOLID | ObjectFilterType::SCENE_OBJECT
            };

            enum ShapeType{
                NO_SHAPE,
                CIRCLE,
                CAPSULE,
                RECTANGLE,
                POLYGON,
                SEGMENT
            };

            b2BodyId& create(std::string world, std::string bodyName, std::shared_ptr<game::Object> object, sf::Vector2f position, b2BodyType type = b2_staticBody, bool fixRotation = true);

            b2ShapeId createShapeCircle(std::string world, std::string bodyName, sf::Vector2f center, float radius, float density = 0.03, float friction = 0.7, float restitution = 0.0);
            b2ShapeId createShapeCapsule(std::string world, std::string bodyName, sf::Vector2f center1, sf::Vector2f center2, float radius, float density = 0.03, float friction = 0.7, float restitution = 0.0);
            b2ShapeId createShapePolygon(std::string world, std::string bodyName, std::vector<b2Vec2> points, float rounded = 0.0, sf::Vector2f offset = {.0, .0}, float density = 0.03, float friction = 0.7, float restitution = 0.0);
            b2ShapeId createShapeRectangle(std::string world, std::string bodyName, sf::Vector2f size, float rounded = 0.0, float rotationDeg = 0.0 , sf::Vector2f offset = {.0, .0}, float density = 0.03, float friction = 0.7, float restitution = 0.0);
            b2ShapeId createShapeSegment(std::string world, std::string bodyName, sf::Vector2f point1, sf::Vector2f point2, float density = 0.03, float friction = 0.7, float restitution = 0.0);

            b2ShapeId createShapeCircle(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f center, float radius, float density = 0.03, float friction = 0.7, float restitution = 0.0);
            b2ShapeId createShapeCapsule(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f center1, sf::Vector2f center2, float radius, float density = 0.03, float friction = 0.7, float restitution = 0.0);
            b2ShapeId createShapePolygon(std::string world, std::shared_ptr<game::Object> object, std::vector<b2Vec2> points, float rounded = 0.0, sf::Vector2f offset = {.0, .0}, float density = 0.03, float friction = 0.7, float restitution = 0.0);
            b2ShapeId createShapeRectangle(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f size, float rounded = 0.0, float rotationDeg = 0.0 , sf::Vector2f offset = {.0, .0}, float density = 0.03, float friction = 0.7, float restitution = 0.0);
            b2ShapeId createShapeSegment(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f point1, sf::Vector2f point2, float density = 0.03, float friction = 0.7, float restitution = 0.0);


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
            ObjectFilterType getFilterMask(std::string world, std::string body);
            ObjectFilterType getFilterCategory(std::string world, std::string body);

            sf::Vector2f getPosition(std::string world, std::shared_ptr<game::Object> object);
            sf::Vector2f getVelocity(std::string world, std::shared_ptr<game::Object> object);
            // sf::Vector2f getSize(std::string world, std::shared_ptr<game::Object> object);
            b2BodyType getType(std::string world, std::shared_ptr<game::Object> object);
            float getDensity(std::string world, std::shared_ptr<game::Object> object);
            float getFriction(std::string world, std::shared_ptr<game::Object> object);
            float getRestitution(std::string world, std::shared_ptr<game::Object> object);
            bool getFixRotation(std::string world, std::shared_ptr<game::Object> object);
            ObjectFilterType getFilterMask(std::string world, std::shared_ptr<game::Object> object);
            ObjectFilterType getFilterCategory(std::string world, std::shared_ptr<game::Object> object);

            void setPosition(std::string world, std::string body, sf::Vector2f position);
            void setVelocity(std::string world, std::string body, sf::Vector2f velocity);
            // void setSize(std::string world, std::string body, sf::Vector2f size);
            void setType(std::string world, std::string body, b2BodyType type);
            void setDensity(std::string world, std::string body, float density);
            void setFriction(std::string world, std::string body, float friction);
            void setRestitution(std::string world, std::string body, float restitution);
            void setFixRotation(std::string world, std::string body, bool fixRotation);
            void setFilterMask(std::string world, std::string body, ObjectFilterType objectFilterType);
            void setFilterCategory(std::string world, std::string body, ObjectFilterType objectFilterType, bool autoSet = true);

            void setPosition(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f position);
            void setVelocity(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f velocity);
            // void setSize(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f size);
            void setType(std::string world, std::shared_ptr<game::Object> object, b2BodyType type);
            void setDensity(std::string world, std::shared_ptr<game::Object> object, float density);
            void setFriction(std::string world, std::shared_ptr<game::Object> object, float friction);
            void setRestitution(std::string world, std::shared_ptr<game::Object> object, float restitution);
            void setFixRotation(std::string world, std::shared_ptr<game::Object> object, bool fixRotation);
            void setFilterMask(std::string world, std::shared_ptr<game::Object> object, ObjectFilterType objectFilterType);
            void setFilterCategory(std::string world, std::shared_ptr<game::Object> object, ObjectFilterType objectFilterType, bool autoSet = true);
        }
    }
}

#endif