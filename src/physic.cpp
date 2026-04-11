#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdint>
#include <cstdlib>
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

bool WorldPreSolve(b2ShapeId shapeIdA, b2ShapeId shapeIdB, b2Vec2 point, b2Vec2 normal, void* context){
    b2BodyId bodyA = b2Shape_GetBody(shapeIdA);
    b2BodyId bodyB = b2Shape_GetBody(shapeIdB);
    // if(b2Body_GetType(bodyA) != b2_dynamicBody) return false;
    if(!context) return false;

    game::Object* A = (game::Object*) b2Body_GetUserData(bodyA);
    game::Object* B = (game::Object*) b2Body_GetUserData(bodyB);

    // if(A) std::cout << "A: " << A->getBodyName() << ' ';
    // if(B) std::cout << "B: " << B->getBodyName() << ' ';

    // std::cout << normal.x << " " << normal.y << ' ';
    // std::cout << point.x << " " << point.y << std::endl;

    return true;
}

namespace game{
    namespace physic{
        namespace world{
            b2WorldId& create(double gravityX, double gravityY, std::string world){
                if(worldMap.contains(world)) return worldMap[world].id;

                worldMap[world].def = b2DefaultWorldDef();
                worldMap[world].def.gravity.x = gravityX;
                worldMap[world].def.gravity.y = gravityY;
                worldMap[world].def.userData = calloc(sizeof(void*), 1);

                worldMap[world].id = b2CreateWorld(&worldMap[world].def);
                b2World_SetPreSolveCallback(worldMap[world].id, WorldPreSolve, worldMap[world].def.userData);

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
                        if(it.second.def.userData) free(it.second.def.userData);
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
            void eventBeginTouch(std::string world){
                b2ContactEvents contact = b2World_GetContactEvents(worldMap[world].id);
                for(int i=0; i<contact.beginCount; i++){
                    b2ContactBeginTouchEvent& beginEvent = contact.beginEvents[i];
                    game::Object* A = (game::Object*) b2Body_GetUserData(b2Shape_GetBody(beginEvent.shapeIdA));
                    game::Object* B = (game::Object*) b2Body_GetUserData(b2Shape_GetBody(beginEvent.shapeIdB));

                    std::cout << "Begin Contatic" << std::endl;
                    if(A) std::cout << "A: " << A->getBodyName() << std::endl;
                    else std::cout << "A: " << "Ground" << std::endl;
                    if(B) std::cout << "B: " << B->getBodyName() << std::endl;
                    else std::cout << "B: " << "Ground" << std::endl;
                }
            }
            void eventEndTouch(std::string world){
                b2ContactEvents contact = b2World_GetContactEvents(worldMap[world].id);
                for(int i=0; i<contact.endCount; i++){
                    b2ContactEndTouchEvent& endEvent = contact.endEvents[i];
                    game::Object* A = (game::Object*) b2Body_GetUserData(b2Shape_GetBody(endEvent.shapeIdA));
                    game::Object* B = (game::Object*) b2Body_GetUserData(b2Shape_GetBody(endEvent.shapeIdB));

                    std::cout << "End Contatic" << std::endl;
                    if(A) std::cout << "A: " << A->getBodyName() << std::endl;
                    else std::cout << "A: " << "Ground" << std::endl;
                    if(B) std::cout << "B: " << B->getBodyName() << std::endl;
                    else std::cout << "B: " << "Ground" << std::endl;
                }
            }
            /* This function is used to detect collision with high speed between 2 shapes */
            void eventHit(std::string world){
                b2ContactEvents contact = b2World_GetContactEvents(worldMap[world].id);
                for(int i=0; i<contact.hitCount; i++){
                    b2ContactHitEvent& hitEvent = contact.hitEvents[i];

                    game::Object* A = (game::Object*) b2Body_GetUserData(b2Shape_GetBody(hitEvent.shapeIdA));
                    game::Object* B = (game::Object*) b2Body_GetUserData(b2Shape_GetBody(hitEvent.shapeIdB));
                    // std::cout << "###### Start Hit ######" << std::endl;
                    // if(A) std::cout << "A: " << A->getBodyName() << std::endl;
                    // else std::cout << "A: " << "Ground" << std::endl;
                    // if(B) std::cout << "B: " << B->getBodyName() << std::endl;
                    // else std::cout << "B: " << "Ground" << std::endl;

                    // std::cout << hitEvent.approachSpeed << "\n";
                    // if(hitEvent.approachSpeed > 70){
                    //     std::cout << "Damage" << std::endl;
                    // }else if(hitEvent.approachSpeed > 40){
                    //     std::cout << "HIT" << std::endl;
                    // }
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
                        b2ShapeType shapeType = b2Shape_GetType(shapes[i]);

                        if(shapeType == b2_capsuleShape){
                            b2Capsule capsule = b2Shape_GetCapsule(shapes[i]);
                            sf::CircleShape sfCircle1;
                            sf::CircleShape sfCircle2;

                            sfCircle1.setRadius(capsule.radius/2);
                            sfCircle2.setRadius(capsule.radius/2);
                            sfCircle1.setPosition({position.x + capsule.center1.x, position.y + capsule.center1.y});
                            sfCircle2.setPosition({position.x + capsule.center2.x, position.y + capsule.center2.y});
                            sfCircle1.setFillColor(sf::Color(0x00000000));
                            sfCircle2.setFillColor(sf::Color(0x00000000));
                            sfCircle1.setOutlineThickness(1);
                            sfCircle2.setOutlineThickness(1);
                            sfCircle1.setOutlineColor(sf::Color(0xffffffff));
                            sfCircle2.setOutlineColor(sf::Color(0xffffffff));

                            render.draw(sfCircle1);
                            render.draw(sfCircle2);
                        }else if(shapeType == b2_circleShape){
                            b2Circle circle = b2Shape_GetCircle(shapes[i]);
                            sf::CircleShape sfCircle;

                            sfCircle.setRadius(circle.radius/2);
                            sfCircle.setPosition({position.x, position.y});
                            sfCircle.setFillColor(sf::Color(0x00000000));
                            sfCircle.setOutlineThickness(1);
                            sfCircle.setOutlineColor(sf::Color(0xffffffff));

                            render.draw(sfCircle);
                        }else if(shapeType == b2_polygonShape){
                            b2Polygon poligon = b2Shape_GetPolygon(shapes[i]);
                            sf::VertexArray lines(sf::PrimitiveType::LineStrip, poligon.count+1);

                            for(int j=0; j<poligon.count; j++){
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
                        } else if(shapeType == b2_segmentShape){
                            b2Segment segment = b2Shape_GetSegment(shapes[i]);
                            sf::VertexArray lines(sf::PrimitiveType::Lines, 2);

                            lines[0].position = sf::Vector2f({segment.point1.x/2.0f, segment.point1.y/2.0f});
                            lines[0].color = sf::Color::White;
                            lines[1].position = sf::Vector2f({segment.point2.x/2.0f, segment.point2.y/2.0f});
                            lines[1].color = sf::Color::White;
                            render.draw(lines);
                        }
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
            b2BodyId& create(std::string world, std::string bodyName, std::shared_ptr<game::Object> object, sf::Vector2f position, b2BodyType type, bool fixRotation){
                if(!bodyMap[world].contains(bodyName)){
                    if(object){
                        object->setBodyName(bodyName);
                        object->setWorldName(world);
                    }

                    bodyMap[world][bodyName].def = b2DefaultBodyDef();
                    bodyMap[world][bodyName].def.position = (b2Vec2) {position.x, position.y};
                    bodyMap[world][bodyName].def.type = type;
                    bodyMap[world][bodyName].def.motionLocks.angularZ = fixRotation;

                    bodyMap[world][bodyName].id = b2CreateBody(worldMap[world].id, &bodyMap[world][bodyName].def);
                    if(object) b2Body_SetUserData(bodyMap[world][bodyName].id, static_cast<void*>(object.get()));
                }

                return bodyMap[world][bodyName].id;
            }

            b2ShapeId createShapeCircle(std::string world, std::string bodyName, sf::Vector2f center, float radius, float density, float friction, float restitution){
                b2BodyId bodyId = bodyMap[world][bodyName].id;
                b2ShapeDef shapeDef = b2DefaultShapeDef();
                game::Object* object;
                if((object = getObject(world, bodyName)) == nullptr){
                    shapeDef.filter.categoryBits = ObjectFilterType::GROUND;
                    shapeDef.filter.maskBits = ObjectFilterTypeMask::GROUND_MASK;
                }else{
                    shapeDef.filter.categoryBits = ObjectFilterType::SCENE_OBJECT;
                    shapeDef.filter.maskBits = ObjectFilterTypeMask::SCENE_OBJECT_MASK;
                }

                b2Circle circle = {{center.x, center.y}, radius};

                b2ShapeId shapeId = b2CreateCircleShape(bodyId, &shapeDef, &circle);
                b2Shape_SetFriction(shapeId, friction);
                b2Shape_SetRestitution(shapeId, restitution);
                b2Shape_SetDensity(shapeId, density, true);
                b2Shape_EnableContactEvents(shapeId, (object != nullptr));
                b2Shape_EnableHitEvents(shapeId, (object != nullptr));
                b2Shape_EnablePreSolveEvents(shapeId, (object != nullptr));
                // b2Shape_EnableSensorEvents(shapeId, (object != nullptr));

                b2Body_SetSleepThreshold(bodyMap[world][bodyName].id, 0.0); // Make object always waked
                return shapeId;
            }
            b2ShapeId createShapeCapsule(std::string world, std::string bodyName, sf::Vector2f center1, sf::Vector2f center2, float radius, float density, float friction, float restitution){
                b2BodyId bodyId = bodyMap[world][bodyName].id;
                b2ShapeDef shapeDef = b2DefaultShapeDef();
                game::Object* object;
                if((object = getObject(world, bodyName)) == nullptr){
                    shapeDef.filter.categoryBits = ObjectFilterType::GROUND;
                    shapeDef.filter.maskBits = ObjectFilterTypeMask::GROUND_MASK;
                }else{
                    shapeDef.filter.categoryBits = ObjectFilterType::SCENE_OBJECT;
                    shapeDef.filter.maskBits = ObjectFilterTypeMask::SCENE_OBJECT_MASK;
                }

                b2Capsule capsule = {{center1.x, center1.y}, {center2.x, center2.y}, radius};

                b2ShapeId shapeId = b2CreateCapsuleShape(bodyId, &shapeDef,&capsule);
                b2Shape_SetFriction(shapeId, friction);
                b2Shape_SetRestitution(shapeId, restitution);
                b2Shape_SetDensity(shapeId, density, true);
                b2Shape_EnableContactEvents(shapeId, (object != nullptr));
                b2Shape_EnableHitEvents(shapeId, (object != nullptr));
                b2Shape_EnablePreSolveEvents(shapeId, (object != nullptr));
                // b2Shape_EnableSensorEvents(shapeId, (object != nullptr));

                b2Body_SetSleepThreshold(bodyMap[world][bodyName].id, 0.0); // Make object always waked
                return shapeId;
            }
            b2ShapeId createShapePolygon(std::string world, std::string bodyName, std::vector<b2Vec2> points, float rounded, sf::Vector2f offset, float density, float friction, float restitution){
                b2BodyId bodyId = bodyMap[world][bodyName].id;
                b2ShapeDef shapeDef = b2DefaultShapeDef();
                game::Object* object;
                if((object = getObject(world, bodyName)) == nullptr){
                    shapeDef.filter.categoryBits = ObjectFilterType::GROUND;
                    shapeDef.filter.maskBits = ObjectFilterTypeMask::GROUND_MASK;
                }else{
                    shapeDef.filter.categoryBits = ObjectFilterType::SCENE_OBJECT;
                    shapeDef.filter.maskBits = ObjectFilterTypeMask::SCENE_OBJECT_MASK;
                }

                b2Rot rot = b2MakeRot(0);
                b2Hull hull = b2ComputeHull(points.data(), points.size());
                b2Polygon polygon = b2MakeOffsetRoundedPolygon(&hull, (b2Vec2){offset.x, offset.y}, rot, rounded);

                b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &polygon);
                b2Shape_SetRestitution(shapeId, restitution);
                b2Shape_SetDensity(shapeId, density, true);
                b2Shape_SetFriction(shapeId, friction);
                b2Shape_EnableContactEvents(shapeId, (object != nullptr));
                b2Shape_EnableHitEvents(shapeId, (object != nullptr));
                b2Shape_EnablePreSolveEvents(shapeId, (object != nullptr));
                // b2Shape_EnableSensorEvents(shapeId, (object != nullptr));

                b2Body_SetSleepThreshold(bodyMap[world][bodyName].id, 0.0); // Make object always waked
                return shapeId;
            }
            b2ShapeId createShapeRectangle(std::string world, std::string bodyName, sf::Vector2f size, float rounded, float rotationDeg, sf::Vector2f offset, float density, float friction, float restitution){
                b2BodyId bodyId = bodyMap[world][bodyName].id;
                b2ShapeDef shapeDef = b2DefaultShapeDef();
                game::Object* object;
                if((object = getObject(world, bodyName)) == nullptr){
                    shapeDef.filter.categoryBits = ObjectFilterType::GROUND;
                    shapeDef.filter.maskBits = ObjectFilterTypeMask::GROUND_MASK;
                }else{
                    shapeDef.filter.categoryBits = ObjectFilterType::SCENE_OBJECT;
                    shapeDef.filter.maskBits = ObjectFilterTypeMask::SCENE_OBJECT_MASK;
                }

                b2Rot rot = b2MakeRot(rotationDeg * (B2_PI / 180.0f));
                b2Polygon polygon = b2MakeOffsetRoundedBox(size.x, size.y, {offset.x, offset.y}, rot, rounded);
                b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &polygon);
                b2Shape_SetRestitution(shapeId, restitution);
                b2Shape_SetDensity(shapeId, density, true);
                b2Shape_SetFriction(shapeId, friction);
                b2Shape_EnableContactEvents(shapeId, (object != nullptr));
                b2Shape_EnableHitEvents(shapeId, (object != nullptr));
                b2Shape_EnablePreSolveEvents(shapeId, (object != nullptr));
                // b2Shape_EnableSensorEvents(shapeId, (object != nullptr));

                b2Body_SetSleepThreshold(bodyMap[world][bodyName].id, 0.0); // Make object always waked
                return shapeId;
            }

            b2ShapeId createShapeSegment(std::string world, std::string bodyName, sf::Vector2f point1, sf::Vector2f point2, float density, float friction, float restitution){
                b2BodyId bodyId = bodyMap[world][bodyName].id;
                b2ShapeDef shapeDef = b2DefaultShapeDef();
                game::Object* object;
                if((object = getObject(world, bodyName)) == nullptr){
                    shapeDef.filter.categoryBits = ObjectFilterType::GROUND;
                    shapeDef.filter.maskBits = ObjectFilterTypeMask::GROUND_MASK;
                }else{
                    shapeDef.filter.categoryBits = ObjectFilterType::SCENE_OBJECT;
                    shapeDef.filter.maskBits = ObjectFilterTypeMask::SCENE_OBJECT_MASK;
                }

                b2Segment segment = {{point1.x, point1.y}, {point2.x, point2.y}};
                b2ShapeId shapeId = b2CreateSegmentShape(bodyId, &shapeDef, &segment);
                b2Shape_SetRestitution(shapeId, restitution);
                b2Shape_SetDensity(shapeId, density, true);
                b2Shape_SetFriction(shapeId, friction);
                b2Shape_EnableContactEvents(shapeId, (object != nullptr));
                b2Shape_EnableHitEvents(shapeId, (object != nullptr));
                b2Shape_EnablePreSolveEvents(shapeId, (object != nullptr));
                // b2Shape_EnableSensorEvents(shapeId, (object != nullptr));

                b2Body_SetSleepThreshold(bodyMap[world][bodyName].id, 0.0); // Make object always waked
                return shapeId;
            }

            b2ShapeId createShapeCircle(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f center, float radius, float density, float friction, float restitution){
                return createShapeCircle(world, object->getBodyName(), center, radius, density, friction, restitution);
            }
            b2ShapeId createShapeCapsule(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f center1, sf::Vector2f center2, float radius, float density, float friction, float restitution){
                return createShapeCapsule(world, object->getBodyName(), center1, center2, radius, density, friction, restitution);
            }
            b2ShapeId createShapePolygon(std::string world, std::shared_ptr<game::Object> object, std::vector<b2Vec2> points, float rounded,  sf::Vector2f offset, float density, float friction, float restitution){
                return createShapePolygon(world, object->getBodyName(), points, rounded, offset, density, friction, restitution);
            }
            b2ShapeId createShapeRectangle(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f size, float radius, float rotationDeg , sf::Vector2f offset, float density, float friction, float restitution){
                return createShapeRectangle(world, object->getBodyName(), size, radius, rotationDeg, offset, density, friction, restitution);
            }
            b2ShapeId createShapeSegment(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f point1, sf::Vector2f point2, float density, float friction, float restitution){
                return createShapeSegment(world, object->getBodyName(), point1, point2, density, friction, restitution);
            }

            b2BodyId& get(std::string world, std::shared_ptr<game::Object> object){
                return bodyMap[world][object->getBodyName()].id;
            }
            b2BodyId& get(std::string world, std::string body){
                return bodyMap[world][body].id;
            }

            void destroy(std::string world, std::shared_ptr<game::Object> object){
                destroy(world, object->getBodyName());
            }
            void destroy(std::string world, std::string body){
                b2DestroyBody(bodyMap[world][body].id);
                bodyMap[world].erase(body);
            }
            bool exists(std::string world, std::shared_ptr<game::Object> object){
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
            sf::Vector2f getVelocity(std::string world, std::string body){
                b2Vec2 vec = b2Body_GetLinearVelocity(bodyMap[world][body].id);
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
                return bodyMap[world][body].def.motionLocks.angularZ;
            }
            ObjectFilterType getFilterMask(std::string world, std::string body){
                b2ShapeId shape;
                b2Body_GetShapes(bodyMap[world][body].id, &shape, 1);
                return (ObjectFilterType) b2Shape_GetFilter(shape).maskBits;
            }
            ObjectFilterType getFilterCategory(std::string world, std::string body){
                b2ShapeId shape;
                b2Body_GetShapes(bodyMap[world][body].id, &shape, 1);
                return (ObjectFilterType) b2Shape_GetFilter(shape).categoryBits;
            }

            sf::Vector2f getPosition(std::string world, std::shared_ptr<game::Object> object){
                return getPosition(world, object->getBodyName());
            }
            sf::Vector2f getVelocity(std::string world, std::shared_ptr<game::Object> object){
                return getVelocity(world, object->getBodyName());
            }
            // sf::Vector2f getSize(std::string world, std::shared_ptr<game::Object> object){
            // }
            b2BodyType getType(std::string world, std::shared_ptr<game::Object> object){
                return getType(world, object->getBodyName());
            }
            float getDensity(std::string world, std::shared_ptr<game::Object> object){
                return getDensity(world, object->getBodyName());
            }
            float getFriction(std::string world, std::shared_ptr<game::Object> object){
                return getFriction(world, object->getBodyName());
            }
            float getRestitution(std::string world, std::shared_ptr<game::Object> object){
                return getRestitution(world, object->getBodyName());
            }
            bool getFixRotation(std::string world, std::shared_ptr<game::Object> object){
                return getFixRotation(world, object->getBodyName());
            }
            ObjectFilterType getFilterMask(std::string world, std::shared_ptr<game::Object> object){
                return getFilterMask(world, object->getBodyName());
            }
            ObjectFilterType getFilterCategory(std::string world, std::shared_ptr<game::Object> object){
                return getFilterCategory(world, object->getBodyName());
            }

            void setPosition(std::string world, std::string body, sf::Vector2f position){
                b2Rot rot = b2Body_GetRotation(bodyMap[world][body].id);
                b2Body_SetTransform(bodyMap[world][body].id, (b2Vec2){position.x, position.y}, rot);
            }
            void setVelocity(std::string world, std::string body, sf::Vector2f velocity){
                b2Body_SetLinearVelocity(bodyMap[world][body].id, (b2Vec2){velocity.x, velocity.y});
            }
            // void setSize(std::string world, std::string body, sf::Vector2f size){
            // }
            void setType(std::string world, std::string body, b2BodyType type){
                b2Body_SetType(bodyMap[world][body].id, type);
            }
            void setDensity(std::string world, std::string body, float density){
                b2ShapeId shape;
                b2Body_GetShapes(bodyMap[world][body].id, &shape, 1);
                b2Shape_SetDensity(shape, density, true);
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
                bodyMap[world][body].def.motionLocks.angularZ = fixRotation;
            }
            void setFilterMask(std::string world, std::string body, ObjectFilterType objectFilterType){
                b2ShapeId shape;
                b2Filter filter;
                b2Body_GetShapes(bodyMap[world][body].id, &shape, 1);
                filter = b2Shape_GetFilter(shape);
                filter.maskBits = (uint64_t) objectFilterType;
                b2Shape_SetFilter(shape, filter);
            }
            void setFilterCategory(std::string world, std::string body, ObjectFilterType objectFilterType, bool autoSet){
                b2ShapeId shape;
                b2Filter filter;
                b2Body_GetShapes(bodyMap[world][body].id, &shape, 1);
                filter = b2Shape_GetFilter(shape);
                filter.categoryBits = (uint64_t) objectFilterType;
                if(autoSet){
                    switch(objectFilterType){
                        case NO_BODY:
                        filter.maskBits = NO_BODY_MASK;
                        break;
                        case GROUND:
                        filter.maskBits = GROUND_MASK;
                        break;
                        case SOLID:
                        filter.maskBits = SOLID_MASK;
                        break;
                        case SCENE_OBJECT:
                        filter.maskBits = SCENE_OBJECT_MASK;
                        break;
                        case GHOST:
                        filter.maskBits = GHOST_MASK;
                        break;
                        case ATTACK:
                        filter.maskBits = ATTACK_MASK;
                        break;
                        case GHOST_ATTACK:
                        filter.maskBits = GHOST_ATTACK_MASK;
                        break;
                        default:
                        filter.maskBits = NO_BODY_MASK;
                    }
                }
                b2Shape_SetFilter(shape, filter);
            }

            void setPosition(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f position){
                setPosition(world, object->getBodyName(), position);
            }
            void setVelocity(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f velocity){
                setVelocity(world, object->getBodyName(), velocity);
            }
            // void setSize(std::string world, std::shared_ptr<game::Object> object, sf::Vector2f size){
            // }
            void setType(std::string world, std::shared_ptr<game::Object> object, b2BodyType type){
                setType(world, object->getBodyName(), type);
            }
            void setDensity(std::string world, std::shared_ptr<game::Object> object, float density){
                setDensity(world, object->getBodyName(), density);
            }
            void setFriction(std::string world, std::shared_ptr<game::Object> object, float friction){
                setFriction(world, object->getBodyName(), friction);
            }
            void setRestitution(std::string world, std::shared_ptr<game::Object> object, float restitution){
                setRestitution(world, object->getBodyName(), restitution);
            }
            void setFixRotation(std::string world, std::shared_ptr<game::Object> object, bool fixRotation){
                setFixRotation(world, object->getBodyName(), fixRotation);
            }
            void setFilterMask(std::string world, std::shared_ptr<game::Object> object, ObjectFilterType objectFilterType){
                setFilterMask(world, object->getBodyName(), objectFilterType);
            }
            void setFilterCategory(std::string world, std::shared_ptr<game::Object> object, ObjectFilterType objectFilterType, bool autoSet){
                setFilterCategory(world, object->getBodyName(), objectFilterType, autoSet);
            }
        }
    }
}