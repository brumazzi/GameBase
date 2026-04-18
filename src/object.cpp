#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <object.hpp>
#include <physic.hpp>
#include <resource.hpp>
#include "animation.hpp"

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

                if(this->m_friction > 0.7) this->setFriction(this->m_friction-5);
                else if(this->m_friction < 0.7) this->setFriction(0.7f);
                if(this->m_friction > 0.7) std::cout << this->m_friction << std::endl;

                if(this->m_grounded){
                    sf::Vector2f velocity = game::physic::body::getVelocity(this);
                    if((std::abs((int)velocity.y)) > 0.1) this->m_grounded = false;
                }
            }

            this->m_animation.second->update();

            // if (this->m_animations.contains(this->m_animation)) {
            //     if (!this->m_animationDelayCur) {
            //         this->m_animationDelayCur = this->m_animationDelay + 1;
            //         this->m_animationIndex = (this->m_animationIndex + 1) % this->m_animations[this->m_animation].size();
            //         this->m_sprite->setTextureRect(this->m_animations[this->m_animation][this->m_animationIndex]);
            //     }
            //     this->m_animationDelayCur--;
            // }
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
        if(!this->m_grounded) return;

        physic::body::applyForce(this->getWorldName(), this->getBodyName(), {0.0, -game::physic::body::BASE_FORCE*this->m_force});
    }

    // AnimationMap Object::animations() { return this->m_animations; }
    Animation::Ptr Object::createAnimation(std::string key, std::pair<std::string, sf::Texture*> texture){
        auto animation = Animation::create(texture);
        this->m_animations[key] = animation;
        return animation;
    }
    // void Object::addAnimation(std::string key, sf::IntRect rect) { this->m_animations[key].emplace_back(rect); }
    void Object::removeAnimation(std::string key) {
        if (this->m_animations.contains(key)) {
            this->m_animations.erase(key);
        }
    }
    // void Object::removeAnimationFrame(std::string key, unsigned int frame) {
    //     if (this->m_animations.contains(key)) {
    //         if (this->m_animations[key].size() > frame) {
    //             this->m_animations[key].erase(this->m_animations[key].begin() + frame);
    //         }
    //     }
    // }
    void Object::setAnimation(std::string key) {
        if(!this->m_animations.contains((key))) return;

        this->m_animation.first = key;
        this->m_animation.second = this->m_animations[key];
        this->m_animation.second->resetFrame();
    }
    std::pair<std::string, Animation::Ptr> Object::getAnimation() { return this->m_animation; }

    // void Object::setDelay(uint16_t delay) {
    //     this->m_animationDelay = delay;
    //     this->m_animationDelayCur = delay;
    // }
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

    // uint16_t Object::getDelay() { return this->m_animationDelay; }
    std::string Object::getBodyName() { return this->m_bodyName; }
    std::string Object::getWorldName() { return this->m_worldName; }
    std::string Object::getTextureName() { return this->m_texture; }
    sf::Vector2f Object::getSize() { return this->m_size; }
    Object::Type Object::getType(){ return this->m_type; }
    float Object::getForce(){ return this->m_force; }
    float Object::getFriction(){ return this->m_friction; }

    Object::operator sf::Sprite() { return (sf::Sprite) this->m_animation.second->sprite(); }
}  // namespace game