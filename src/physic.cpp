#include <physic.hpp>
#include <object.hpp>

#include <map>

#define UPDATE_TIME 1/6.0

typedef struct world{
    b2WorldDef def;
    b2WorldId id;
}World;

typedef struct body{
    b2BodyDef def;
    b2BodyId id;
}Body;

std::map<std::string, World> worldMap;
std::map<std::string, std::map<std::string, Body>> bodyMap;

namespace game{
    namespace physic{
        namespace world{
            b2WorldId& create(double gravityX, double gravityY, std::string world){
                if(worldMap.contains(world)) return worldMap[world].id;

                worldMap[world].def = b2DefaultWorldDef();
                worldMap[world].def.gravity.x = gravityX;
                worldMap[world].def.gravity.y = gravityY;

                worldMap[world].id = b2CreateWorld(&worldMap[world].def);

                return worldMap[world].id;
            }
            b2WorldId& create(sf::Vector2f gravity, std::string world){
                return create(gravity.x, gravity.y, world);
            }
            b2WorldId& get(std::string world){
                return worldMap[world].id;
            }
            void destroy(std::string world){
                if(worldMap.contains(world)){
                    for(auto it: bodyMap[world]){
                        b2DestroyBody(it.second.id);
                    }
                    bodyMap.erase(world);
                    b2DestroyWorld(worldMap[world].id);
                    worldMap.erase(world);
                }
            }
            bool exists(std::string world){
                return worldMap.contains(world);
            }

            void active(std::string world){
                b2World_EnableSleeping(worldMap[world].id, false);
            }
            void desactive(std::string world){
                b2World_EnableSleeping(worldMap[world].id, true);
            }

            sf::Vector2f getGravity(std::string world){
                b2Vec2 gravity = b2World_GetGravity(worldMap[world].id);
                return sf::Vector2f(gravity.x, gravity.y);
            }
            void setGravity(sf::Vector2f gravity, std::string world){
                b2World_SetGravity(worldMap[world].id, (b2Vec2) {gravity.x, gravity.y});
            }

            void update(std::string world){
                if(!world.size()){
                    for(auto it: worldMap){
                        b2World_Step(it.second.id, UPDATE_TIME, 4);
                    }
                }else{
                    b2World_Step(worldMap[world].id, UPDATE_TIME, 4);
                }
            }
            void update(std::vector<std::string> worlds){
                for(auto world: worlds){
                    b2World_Step(worldMap[world].id, UPDATE_TIME, 4);
                }
            }

            void drawLines(sf::RenderWindow& render){
                for(auto it: worldMap){
                    drawLines(render, it.first);
                }
            }
            void drawLines(sf::RenderWindow& render, std::string world){
                for(auto it: bodyMap[world]){
                    ulong shapeCount = b2Body_GetShapeCount(it.second.id);
                    b2Vec2 position = b2Body_GetPosition(it.second.id);

                    b2ShapeId* shapes = new b2ShapeId[shapeCount];
                    b2Body_GetShapes(it.second.id, shapes, shapeCount);

                    for(ulong i=0; i<shapeCount; i++){
                        b2Polygon poligon = b2Shape_GetPolygon(shapes[i]);
                        sf::VertexArray lines(sf::PrimitiveType::LineStrip, poligon.count+1);

                        for(ulong j=0; j<poligon.count; j++){
                            lines[j].position = sf::Vector2f(
                                position.x+poligon.vertices[j].x,
                                position.y+poligon.vertices[j].y
                            );
                            lines[j].color = sf::Color::White;
                        }
                        lines[poligon.count].position = sf::Vector2f(
                            position.x+poligon.vertices[0].x,
                            position.y+poligon.vertices[0].y
                        );
                        lines[poligon.count].color = sf::Color::White;


                        render.draw(lines);
                    }
                }
            }
            void drawLines(sf::RenderWindow& render, std::vector<std::string> worlds){
                for(auto world: worlds){
                    drawLines(render, world);
                }
            }
        }
        namespace body{
            b2BodyId& create(std::string world, std::string bodyName, game::Object* object, sf::Vector2f position, sf::Vector2f size, b2BodyType type, float density, float friction, float restitution, bool fixRotation){
                if(!bodyMap[world].contains(bodyName)){
                    if(object){
                        object->setBodyName(bodyName);
                        object->setWorldName(world);
                    }

                    bodyMap[world][bodyName].def = b2DefaultBodyDef();
                    bodyMap[world][bodyName].def.position = (b2Vec2) {position.x, position.y};
                    bodyMap[world][bodyName].def.type = type;
                    bodyMap[world][bodyName].def.fixedRotation = fixRotation;

                    bodyMap[world][bodyName].id = b2CreateBody(worldMap[world].id, &bodyMap[world][bodyName].def);
                    b2Body_SetUserData(bodyMap[world][bodyName].id, static_cast<void*>(object));

                    b2Polygon box = b2MakeBox(size.x, size.y);
                    b2ShapeDef shapeDef = b2DefaultShapeDef();
                    b2ShapeId shape = b2CreatePolygonShape(bodyMap[world][bodyName].id, &shapeDef, &box);
                    b2Shape_SetFriction(shape, friction);
                    b2Shape_SetRestitution(shape, restitution);
                    b2Shape_SetDensity(shape, density);
                    b2Shape_EnableContactEvents(shape, true);
                    b2Shape_EnableHitEvents(shape, true);
                    b2Shape_EnablePreSolveEvents(shape, true);
                    b2Shape_EnableSensorEvents(shape, true);

                    b2Body_SetSleepThreshold(bodyMap[world][bodyName].id, 0.0); // Make object always waked

                    return bodyMap[world][bodyName].id;
                }

                return bodyMap[world][bodyName].id;
            }
            b2BodyId& get(std::string world, game::Object* object){
                return bodyMap[world][object->getBodyName()].id;
            }
            b2BodyId& get(std::string world, std::string body){
                return bodyMap[world][body].id;
            }

            void destroy(std::string world, game::Object* object){
                destroy(world, object->getBodyName());
            }
            void destroy(std::string world, std::string body){
                b2DestroyBody(bodyMap[world][body].id);
                bodyMap[world].erase(body);
            }
            bool exists(std::string world, game::Object* object){
                return bodyMap[world].contains(object->getBodyName());
            }
            bool exists(std::string world, std::string body){
                return bodyMap[world].contains(body);
            }

            game::Object* getObject(std::string world, std::string body){
                return static_cast<game::Object*>(b2Body_GetUserData(bodyMap[world][body].id));
            }
            sf::Vector2f getPosition(std::string world, std::string body){
                b2Vec2 vec = b2Body_GetPosition(bodyMap[world][body].id);
                return sf::Vector2f(vec.x, vec.y);
            }
            // sf::Vector2f getSize(std::string world, std::string body){
            // }
            b2BodyType getType(std::string world, std::string body){
                return b2Body_GetType(bodyMap[world][body].id);
            }
            float getDensity(std::string world, std::string body){
                b2ShapeId shape;
                b2Body_GetShapes(bodyMap[world][body].id, &shape, 1);
                return b2Shape_GetDensity(shape);
            }
            float getFriction(std::string world, std::string body){
                b2ShapeId shape;
                b2Body_GetShapes(bodyMap[world][body].id, &shape, 1);
                return b2Shape_GetFriction(shape);
            }
            float getRestitution(std::string world, std::string body){
                b2ShapeId shape;
                b2Body_GetShapes(bodyMap[world][body].id, &shape, 1);
                return b2Shape_GetRestitution(shape);
            }
            bool getFixRotation(std::string world, std::string body){
                return b2Body_IsFixedRotation(bodyMap[world][body].id);
            }

            sf::Vector2f getPosition(std::string world, game::Object* object){
                return getPosition(world, object->getBodyName());
            }
            // sf::Vector2f getSize(std::string world, game::Object* object){
            // }
            b2BodyType getType(std::string world, game::Object* object){
                return getType(world, object->getBodyName());
            }
            float getDensity(std::string world, game::Object* object){
                return getDensity(world, object->getBodyName());
            }
            float getFriction(std::string world, game::Object* object){
                return getFriction(world, object->getBodyName());
            }
            float getRestitution(std::string world, game::Object* object){
                return getRestitution(world, object->getBodyName());
            }
            bool getFixRotation(std::string world, game::Object* object){
                return getFixRotation(world, object->getBodyName());
            }

            void setPosition(std::string world, std::string body, sf::Vector2f position){
                b2Rot rot = b2Body_GetRotation(bodyMap[world][body].id);
                b2Body_SetTransform(bodyMap[world][body].id, (b2Vec2){position.x, position.y}, rot);
            }
            // void setSize(std::string world, std::string body, sf::Vector2f size){
            // }
            void setType(std::string world, std::string body, b2BodyType type){
                b2Body_SetType(bodyMap[world][body].id, type);
            }
            void setDensity(std::string world, std::string body, float density){
                b2ShapeId shape;
                b2Body_GetShapes(bodyMap[world][body].id, &shape, 1);
                b2Shape_SetDensity(shape, density);
            }
            void setFriction(std::string world, std::string body, float friction){
                b2ShapeId shape;
                b2Body_GetShapes(bodyMap[world][body].id, &shape, 1);
                b2Shape_SetFriction(shape, friction);
            }
            void setRestitution(std::string world, std::string body, float restitution){
                b2ShapeId shape;
                b2Body_GetShapes(bodyMap[world][body].id, &shape, 1);
                b2Shape_SetRestitution(shape, restitution);
            }
            void setFixRotation(std::string world, std::string body, bool fixRotation){
                b2Body_SetFixedRotation(bodyMap[world][body].id, fixRotation);
            }

            void setPosition(std::string world, game::Object* object, sf::Vector2f position){
                setPosition(world, object->getBodyName(), position);
            }
            // void setSize(std::string world, game::Object* object, sf::Vector2f size){
            // }
            void setType(std::string world, game::Object* object, b2BodyType type){
                setType(world, object->getBodyName(), type);
            }
            void setDensity(std::string world, game::Object* object, float density){
                setDensity(world, object->getBodyName(), density);
            }
            void setFriction(std::string world, game::Object* object, float friction){
                setFriction(world, object->getBodyName(), friction);
            }
            void setRestitution(std::string world, game::Object* object, float restitution){
                setRestitution(world, object->getBodyName(), restitution);
            }
            void setFixRotation(std::string world, game::Object* object, bool fixRotation){
                setFixRotation(world, object->getBodyName(), fixRotation);
            }
        }
    }
}