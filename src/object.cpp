#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <object.hpp>
#include <physic.hpp>
#include <resource.hpp>

namespace game {
    Object::Ptr Object::create() { return std::make_shared<game::Object>(); }
    Object::Ptr Object::create(std::string texture) { return std::make_shared<game::Object>(texture); }

    Object::Object() {
        this->m_destroyed = false;
        this->m_drawable = false;
        this->m_type = game::object::Type::OBJECT;
    }

    Object::Object(sf::Texture* texture) {
        this->m_sprite = std::make_shared<sf::Sprite>(*texture);
        this->m_animationIndex = 0;
        this->m_destroyed = false;
        this->m_drawable = true;
        this->m_texture = game::resource::texture::mask(texture);
        this->m_type = game::object::Type::OBJECT;
    }
    Object::Object(std::string texture) {
        this->m_sprite = std::make_shared<sf::Sprite>(*resource::texture::get(texture));
        this->m_animationIndex = 0;
        this->m_destroyed = false;
        this->m_drawable = true;
        this->m_texture = texture;
        this->m_type = game::object::Type::OBJECT;
    }
    Object::~Object() {}

    sf::Sprite& Object::sprite() { return *this->m_sprite; }
    void Object::update() {
        if (this->m_destroyed)
            return;

        if (this->m_drawable) {
            if (game::physic::body::exists(this->m_worldName, this->m_bodyName)) {
                sf::Vector2f position(game::physic::body::getPosition(this->m_worldName, this->m_bodyName));
                sf::Vector2f scale = this->m_sprite->getScale();
                sf::IntRect rect = this->m_sprite->getTextureRect();

                position.x -= (rect.size.x * scale.x) / 2;
                position.y -= (rect.size.y * scale.y) / 2;
                this->m_sprite->setPosition(position);
            }

            if (this->m_animations.contains(this->m_animation)) {
                if (!this->m_animationDelayCur) {
                    this->m_animationDelayCur = this->m_animationDelay + 1;
                    this->m_animationIndex = (this->m_animationIndex + 1) % this->m_animations[this->m_animation].size();
                    this->m_sprite->setTextureRect(this->m_animations[this->m_animation][this->m_animationIndex]);
                }
                this->m_animationDelayCur--;
            }
        }

        for(auto [key, callback]: this->m_callbacks){
            if(!callback(this, nullptr)) break;
        }
        // if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)){
        //     game::physic::body::setVelocity("default", "Player", {0.0, -16.0});
        // }
    }
    void Object::destroy() { this->m_destroyed = true; }
    bool Object::isDestroyed() { return this->m_destroyed; }
    bool Object::isDrawable() { return this->m_drawable; }

    AnimationMap Object::animations() { return this->m_animations; }
    void Object::addAnimation(std::string key, sf::IntRect rect) { this->m_animations[key].emplace_back(rect); }
    void Object::removeAnimation(std::string key) {
        if (this->m_animations.contains(key)) {
            this->m_animations.erase(key);
        }
    }
    void Object::removeAnimationFrame(std::string key, unsigned int frame) {
        if (this->m_animations.contains(key)) {
            if (this->m_animations[key].size() > frame) {
                this->m_animations[key].erase(this->m_animations[key].begin() + frame);
            }
        }
    }
    void Object::setAnimation(std::string key) {
        this->m_animation = key;
        this->m_animationIndex = 0;
    }
    std::string Object::getAnimation() { return this->m_animation; }

    void Object::setDelay(uint16_t delay) {
        this->m_animationDelay = delay;
        this->m_animationDelayCur = delay;
    }
    void Object::setBodyName(std::string bodyName) { this->m_bodyName = bodyName; }
    void Object::setWorldName(std::string worldName) { this->m_worldName = worldName; }
    void Object::setSize(sf::Vector2f size) { this->m_size = size; }
    void Object::setType(game::object::Type type) { this->m_type = type; }
    uint16_t Object::getDelay() { return this->m_animationDelay; }
    std::string Object::getBodyName() { return this->m_bodyName; }
    std::string Object::getWorldName() { return this->m_worldName; }
    std::string Object::getTextureName() { return this->m_texture; }
    sf::Vector2f Object::getSize() { return this->m_size; }
    game::object::Type Object::getType(){ return this->m_type; }

    Object::operator sf::Sprite() { return *this->m_sprite; }
}  // namespace game