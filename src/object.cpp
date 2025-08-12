#include <object.hpp>
#include <resource.hpp>
#include <physic.hpp>

namespace game{
    Object::Object(){
        this->m_destroyed = false;
        this->m_drawable = false;
    }

    Object::Object(sf::Texture *texture){
        this->m_sprite = new sf::Sprite(*texture);
        this->m_animationIndex = 0;
        this->m_destroyed = false;
        this->m_drawable = true;
    }
    Object::Object(std::string texture){
        this->m_sprite = new sf::Sprite(*resource::texture::get(texture));
        this->m_animationIndex = 0;
        this->m_destroyed = false;
        this->m_drawable = true;
    }
    Object::~Object(){
        delete this->m_sprite;
    }
    sf::Sprite &Object::sprite(){ return *this->m_sprite; }
    void Object::update(){
        if(this->m_destroyed) return;

        if(this->m_drawable){
            if(game::physic::body::exists(this->m_worldName, this->m_bodyName)){
                sf::Vector2f position(game::physic::body::getPosition(this->m_worldName, this->m_bodyName));
                sf::Vector2f scale = this->m_sprite->getScale();
                sf::IntRect rect = this->m_sprite->getTextureRect();

                position.x -= (rect.size.x*scale.x)/2;
                position.y -= (rect.size.y*scale.y)/2;
                this->m_sprite->setPosition(position);
            }

            if(this->m_animations.contains(this->m_animation)){
                if(!this->m_animationDelayCur){
                    this->m_animationDelayCur = this->m_animationDelay+1;
                    this->m_animationIndex = (this->m_animationIndex+1)%this->m_animations[this->m_animation].size();
                    this->m_sprite->setTextureRect(this->m_animations[this->m_animation][this->m_animationIndex]);
                }
                this->m_animationDelayCur--;
            }
        }
    }
    void Object::destroy(){
        this->m_destroyed = true;
    }
    bool Object::isDestroyed(){
        return this->m_destroyed;
    }
    bool Object::isDrawable(){
        return this->m_drawable;
    }

    void Object::addAnimation(std::string key, sf::IntRect rect){
        this->m_animations[key].emplace_back(rect);
    }
    void Object::removeAnimation(std::string key){
        if(this->m_animations.contains(key)){
            this->m_animations.erase(key);
        }
    }
    void Object::removeAnimationFrame(std::string key, unsigned int frame){
        if(this->m_animations.contains(key)){
            if(this->m_animations[key].size() > frame){
                this->m_animations[key].erase(this->m_animations[key].begin()+frame);
            }
        }
    }
    void Object::setAnimation(std::string key){
        this->m_animation = key;
        this->m_animationIndex = 0;
    }
    std::string Object::getAnimation(){
        return this->m_animation;
    }

    void Object::setDelay(uint16_t delay){
        this->m_animationDelay = delay;
        this->m_animationDelayCur = delay;
    }
    void Object::setBodyName(std::string bodyName){
        this->m_bodyName = bodyName;
    }
    void Object::setWorldName(std::string worldName){
        this->m_worldName = worldName;
    }
    uint16_t Object::getDelay(){
        return this->m_animationDelay;
    }
    std::string Object::getBodyName(){
        return this->m_bodyName;
    }
    std::string Object::getWorldName(){
        return this->m_worldName;
    }

    Object::operator sf::Sprite(){
        return *this->m_sprite;
    }
}