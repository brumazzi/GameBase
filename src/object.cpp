#include <SFML/Graphics.hpp>
#include <object.hpp>
#include <physic.hpp>
#include <resource.hpp>
#include <animation.hpp>
#include <thread>

namespace game {
    Object::Ptr Object::create() { return std::make_shared<Object>(); }

    Object::Object() {
        this->m_destroyed = false;
        this->m_drawable = true;
        this->m_type = Object::Type::OBJECT;
        this->m_grounded = false;
        this->m_force = 17.0f;
    }

    Object::~Object() {
        this->m_animations.clear();

        // b2BodyId bodyId = game::physic::body::get(this);
        // unsigned int shapeCount = b2Body_GetShapeCount(bodyId);
        // b2ShapeId* shapes = new b2ShapeId[shapeCount];
        // b2Body_GetShapes(bodyId, shapes, shapeCount);

        // for(auto [sensor, rect]: this->m_sensors){
        //     for(unsigned int i=0; i<shapeCount; i++){
        //         if(b2Shape_IsSensor(shapes[i])){
        //             void* data = b2Shape_GetUserData(shapes[i]);
        //             if(data) delete data;
        //         }
        //     }
        // }
        // delete shapes;
    }

    sf::Sprite& Object::sprite() { return this->m_animation.second->sprite(); }
    void Object::update() {
        if (this->m_destroyed)
            return;

        if (this->m_drawable && this->m_animations.size()) {
            if (physic::body::exists(this->m_worldName, this->m_bodyName)) {
                sf::Vector2f position(physic::body::getPosition(this->m_worldName, this->m_bodyName));
                sf::Sprite& sprite = this->sprite();

                sf::Vector2f scale = sprite.getScale();
                sf::IntRect rect = sprite.getTextureRect();

                position.x -= (rect.size.x * scale.x) / 2;
                position.y -= (rect.size.y * scale.y) / 2;
                sprite.setPosition(position);

                if(this->m_friction > 0.7) this->m_friction -= 8;
                if(this->m_friction < 0.7) this->m_friction = 0.7;

                this->setFriction(this->m_friction);

                // if(this->m_grounded){
                //     sf::Vector2f velocity = game::physic::body::getVelocity(this);
                //     if((std::abs((int)velocity.y)) > 0.1) this->m_grounded = false;
                // }
            }

            // for(auto [sensor, rect]: this->m_sensors){
            //     game::physic::body::setPosition(this->m_worldName, sensor, game::physic::body::getPosition(this));
            // }

            this->m_animation.second->update();
        }

        for(auto [key, callback]: this->m_callbacks){
            if(!callback(this, nullptr)) break;
        }
        // if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)){
        //     physic::body::setVelocity("default", "Player", {0.0, -16.0});
        // }
    }
    void Object::destroy() { this->m_destroyed = true; }
    bool Object::isDestroyed() { return this->m_destroyed; }
    bool Object::isDrawable() { return this->m_drawable; }
    bool Object::isGrounded() { return this->m_grounded; }
    void Object::jump() {
        if(this->m_grounded){
            physic::body::applyForce(this->m_worldName, this->getBodyName(), {0.0, -game::physic::body::BASE_FORCE*this->m_force});
        }
        if(!this->m_grounded && this->m_friction > 20){
            physic::body::setFriction(this, 0.7);
            physic::body::applyForce(this->m_worldName, this->getBodyName(), {0.0, -game::physic::body::BASE_FORCE*this->m_force});
        }
    }

    Animation::Ptr Object::createAnimation(std::string key, std::pair<std::string, sf::Texture*> texture){
        auto animation = Animation::create(texture);
        this->m_animations[key] = animation;
        return animation;
    }
    void Object::removeAnimation(std::string key) {
        if (this->m_animations.contains(key)) {
            this->m_animations.erase(key);
        }
    }
    void Object::setAnimation(std::string key) {
        if(!this->m_animations.contains((key))) return;

        this->m_animation.first = key;
        this->m_animation.second = this->m_animations[key];
        this->m_animation.second->resetFrame();
    }
    std::pair<std::string, Animation::Ptr> Object::getAnimation() { return this->m_animation; }
    void Object::createSensor(std::string sensorName, sf::Vector2f size, sf::Vector2f offset, game::physic::world::SensorType type){
        // game::physic::body::create(this->m_worldName, sensorName, nullptr, {0, 0}, b2_staticBody, true, this);
        // auto shapeId = game::physic::body::createShapeRectangle(this->getWorldName(), sensorName, size, 0.0, 0.0, offset, 0.01, 0.3, 0.0, true);
        // game::physic::body::setFilterMask(this->m_worldName, sensorName, game::physic::body::ObjectFilterType::GROUND);
        // b2Shape_SetUserData(shapeId, (void*) type);

        auto shapeId = game::physic::body::createShapeRectangle(this->m_worldName, this->m_bodyName, size, 0.0, 0.0, offset, 0.01, 0.3, 0.0, true);
        game::physic::body::setFilterMask(this->m_worldName, this->m_bodyName, game::physic::body::ObjectFilterType::GROUND);
        b2Shape_SetUserData(shapeId, (void*) type);

        this->m_sensors[sensorName] = {offset, size};
    }

    void Object::linkSensor(std::string sensorName, sf::Vector2f offset){ this->m_sensors[sensorName] = {offset, {0,0}}; }
    void Object::unlinkSensor(std::string sensorName){
        if(this->m_sensors.contains(sensorName)){
            this->m_sensors.erase(sensorName);
        }
    }
    void Object::setSensorOffset(std::string sensorName, sf::Vector2f offset){ this->m_sensors[sensorName].position = offset; }
    void Object::enabledSensor(std::string sensorName, bool flag){ game::physic::body::setSensorOn(this->m_worldName, sensorName, flag); }
    sf::FloatRect Object::getSensorRect(std::string sensorName){ return this->m_sensors[sensorName]; }

    void Object::setDrawable(bool drawable){ this->m_drawable = drawable; }
    void Object::setBodyName(std::string bodyName) { this->m_bodyName = bodyName; }
    void Object::setWorldName(std::string worldName) { this->m_worldName = worldName; }
    void Object::setSize(sf::Vector2f size) { this->m_size = size; }
    void Object::setType(Object::Type type) { this->m_type = type; }
    void Object::setForce(float force){ this->m_force = force; }
    void Object::setGrounded(bool grounded){ this->m_grounded = grounded; }
    void Object::setFriction(float friction){
        this->m_friction = friction;
        game::physic::body::setFriction(this, friction);
    }

    std::string Object::getBodyName() { return this->m_bodyName; }
    std::string Object::getWorldName() { return this->m_worldName; }
    std::string Object::getTextureName() { return this->m_texture; }
    sf::Vector2f Object::getSize() { return this->m_size; }
    Object::Type Object::getType(){ return this->m_type; }
    float Object::getForce(){ return this->m_force; }
    float Object::getFriction(){ return this->m_friction; }

    Object::operator sf::Sprite() { return (sf::Sprite) this->m_animation.second->sprite(); }
}  // namespace game