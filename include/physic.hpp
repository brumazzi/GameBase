#ifndef __PHYSIC_HPP__
#define __PHYSIC_HPP__

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <box2d/box2d.h>
#include <box2d/id.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

namespace game{
    class Object;

    namespace physic{
        namespace world{
            typedef enum SensorType{
                NO_SENSOR           = 0,
                GROUNDED_SENSOR     = 1,
            }SensorType;

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
            void eventTouch(std::string world = "default");
            void eventSensor(std::string world = "default");
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
                GHOST_ATTACK    = 1 << 5,
                SENSOR          = 1 << 6
            };

            enum ObjectFilterTypeMask{
                NO_BODY_MASK        = 0,
                GROUND_MASK         = ObjectFilterType::SOLID | ObjectFilterType::SCENE_OBJECT | ObjectFilterType::ATTACK | ObjectFilterType::SENSOR,
                SOLID_MASK          = ObjectFilterType::SOLID | ObjectFilterType::ATTACK | ObjectFilterType::GHOST_ATTACK | ObjectFilterType::GROUND | ObjectFilterType::SENSOR,
                SCENE_OBJECT_MASK   = ObjectFilterType::ATTACK | ObjectFilterType::GHOST_ATTACK | ObjectFilterType::GROUND,
                GHOST_MASK          = ObjectFilterType::ATTACK | ObjectFilterType::GHOST_ATTACK,
                ATTACK_MASK         = ObjectFilterType::SOLID | ObjectFilterType::SCENE_OBJECT | ObjectFilterType::GROUND,
                GHOST_ATTACK_MASK   = ObjectFilterType::SOLID | ObjectFilterType::SCENE_OBJECT,
                SENSOR_MASK         = ObjectFilterType::GROUND | ObjectFilterType::SOLID
            };

            enum ShapeType{
                NO_SHAPE,
                CIRCLE,
                CAPSULE,
                RECTANGLE,
                POLYGON,
                SEGMENT
            };

            const float BASE_FORCE = 460.0f;

            b2BodyId& create(std::string world, std::string bodyName, game::Object* object, sf::Vector2f position, b2BodyType type = b2_staticBody, bool fixRotation = true, void* data = 0x00);

            b2ShapeId createShapeCircle(std::string world, std::string bodyName, sf::Vector2f center, float radius, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2ShapeId createShapeCapsule(std::string world, std::string bodyName, sf::Vector2f center1, sf::Vector2f center2, float radius, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2ShapeId createShapePolygon(std::string world, std::string bodyName, std::vector<b2Vec2> points, float rounded = 0.0, sf::Vector2f offset = {.0, .0}, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2ShapeId createShapeRectangle(std::string world, std::string bodyName, sf::Vector2f size, float rounded = 0.0, float rotationDeg = 0.0 , sf::Vector2f offset = {.0, .0}, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2ShapeId createShapeSegment(std::string world, std::string bodyName, sf::Vector2f point1, sf::Vector2f point2, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);

            b2ShapeId createShapeCircle(game::Object* object, sf::Vector2f center, float radius, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2ShapeId createShapeCapsule(game::Object* object, sf::Vector2f center1, sf::Vector2f center2, float radius, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2ShapeId createShapePolygon(game::Object* object, std::vector<b2Vec2> points, float rounded = 0.0, sf::Vector2f offset = {.0, .0}, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2ShapeId createShapeRectangle(game::Object* object, sf::Vector2f size, float rounded = 0.0, float rotationDeg = 0.0 , sf::Vector2f offset = {.0, .0}, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2ShapeId createShapeSegment(game::Object* object, sf::Vector2f point1, sf::Vector2f point2, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);

            b2BodyId& get(game::Object* object);
            b2BodyId& get(std::string world, std::string body);

            void setSensorOn(std::string world, std::string body, bool flag);

            void destroy(game::Object* object);
            void destroy(std::string world, std::string body);
            bool exists(game::Object* object);
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

            sf::Vector2f getPosition(game::Object* object);
            sf::Vector2f getVelocity(game::Object* object);
            // sf::Vector2f getSize(game::Object* object);
            b2BodyType getType(game::Object* object);
            float getDensity(game::Object* object);
            float getFriction(game::Object* object);
            float getRestitution(game::Object* object);
            bool getFixRotation(game::Object* object);
            ObjectFilterType getFilterMask(game::Object* object);
            ObjectFilterType getFilterCategory(game::Object* object);

            void setPosition(std::string world, std::string body, sf::Vector2f position);
            void setVelocity(std::string world, std::string body, sf::Vector2f velocity);
            void applyForce(std::string world, std::string body, sf::Vector2f force);
            // void setSize(std::string world, std::string body, sf::Vector2f size);
            void setType(std::string world, std::string body, b2BodyType type);
            void setDensity(std::string world, std::string body, float density);
            void setFriction(std::string world, std::string body, float friction);
            void setRestitution(std::string world, std::string body, float restitution);
            void setFixRotation(std::string world, std::string body, bool fixRotation);
            void setFilterMask(std::string world, std::string body, ObjectFilterType objectFilterType);
            void setFilterCategory(std::string world, std::string body, ObjectFilterType objectFilterType, bool autoSet = true);

            void setPosition(game::Object* object, sf::Vector2f position);
            void setVelocity(game::Object* object, sf::Vector2f velocity);
            void applyForce(game::Object* object, sf::Vector2f force);
            // void setSize(game::Object* object, sf::Vector2f size);
            void setType(game::Object* object, b2BodyType type);
            void setDensity(game::Object* object, float density);
            void setFriction(game::Object* object, float friction);
            void setRestitution(game::Object* object, float restitution);
            void setFixRotation(game::Object* object, bool fixRotation);
            void setFilterMask(game::Object* object, ObjectFilterType objectFilterType);
            void setFilterCategory(game::Object* object, ObjectFilterType objectFilterType, bool autoSet = true);

            b2BodyId& create(std::string world, std::string bodyName, std::shared_ptr<game::Object> object, sf::Vector2f position, b2BodyType type = b2_staticBody, bool fixRotation = true, void* data = 0x00);
            b2ShapeId createShapeCircle(std::shared_ptr<game::Object> object, sf::Vector2f center, float radius, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2ShapeId createShapeCapsule(std::shared_ptr<game::Object> object, sf::Vector2f center1, sf::Vector2f center2, float radius, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2ShapeId createShapePolygon(std::shared_ptr<game::Object> object, std::vector<b2Vec2> points, float rounded = 0.0, sf::Vector2f offset = {.0, .0}, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2ShapeId createShapeRectangle(std::shared_ptr<game::Object> object, sf::Vector2f size, float rounded = 0.0, float rotationDeg = 0.0 , sf::Vector2f offset = {.0, .0}, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2ShapeId createShapeSegment(std::shared_ptr<game::Object> object, sf::Vector2f point1, sf::Vector2f point2, float density = 0.03, float friction = 0.7, float restitution = 0.0, bool sensor = false);
            b2BodyId& get(std::shared_ptr<game::Object> object);
            void destroy(std::shared_ptr<game::Object> object);
            bool exists(std::shared_ptr<game::Object> object);
            sf::Vector2f getPosition(std::shared_ptr<game::Object> object);
            sf::Vector2f getVelocity(std::shared_ptr<game::Object> object);
            // sf::Vector2f getSize(std::shared_ptr<game::Object> object);
            b2BodyType getType(std::shared_ptr<game::Object> object);
            float getDensity(std::shared_ptr<game::Object> object);
            float getFriction(std::shared_ptr<game::Object> object);
            float getRestitution(std::shared_ptr<game::Object> object);
            bool getFixRotation(std::shared_ptr<game::Object> object);
            ObjectFilterType getFilterMask(std::shared_ptr<game::Object> object);
            ObjectFilterType getFilterCategory(std::shared_ptr<game::Object> object);
            void setPosition(std::shared_ptr<game::Object> object, sf::Vector2f position);
            void setVelocity(std::shared_ptr<game::Object> object, sf::Vector2f velocity);
            void applyForce(std::shared_ptr<game::Object> object, sf::Vector2f force);
            // void setSize(std::shared_ptr<game::Object> object, sf::Vector2f size);
            void setType(std::shared_ptr<game::Object> object, b2BodyType type);
            void setDensity(std::shared_ptr<game::Object> object, float density);
            void setFriction(std::shared_ptr<game::Object> object, float friction);
            void setRestitution(std::shared_ptr<game::Object> object, float restitution);
            void setFixRotation(std::shared_ptr<game::Object> object, bool fixRotation);
            void setFilterMask(std::shared_ptr<game::Object> object, ObjectFilterType objectFilterType);
            void setFilterCategory(std::shared_ptr<game::Object> object, ObjectFilterType objectFilterType, bool autoSet = true);
        }
    }
}

#endif