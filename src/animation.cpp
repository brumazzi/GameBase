#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <animation.hpp>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

namespace game {
    Animation::Ptr Animation::create(std::pair<std::string, sf::Texture*> texture) { return std::make_shared<game::Animation>(texture); }

    Animation::Animation(std::pair<std::string, sf::Texture*> texture) {
        // memset(this, 0, sizeof(Animation));

        this->m_texture.first = texture.first;
        this->m_texture.second = texture.second;
        this->m_mode = Animation::Mode::DEFAULT;
        this->m_pong = false;
        this->m_delay = 60;
        this->m_delayStep = this->m_delay;
        this->m_stoped = false;
        this->m_sprite = new sf::Sprite(*texture.second);
        this->m_currentFrame = 0;
    }
    Animation::~Animation() {
        delete this->m_sprite;
    }

    void Animation::addFrame(sf::IntRect rect) { this->m_frames.push_back(rect); }
    void Animation::updateFrame(int index, sf::IntRect rect) {
        if(index >= (int) this->m_frames.size()) return;

        this->m_frames[index] = rect;
    }
    void Animation::removeFrame(int index) {
        if(index >= (int) this->m_frames.size()) return;

        this->m_frames.erase(this->m_frames.begin()+index);
    }
    sf::IntRect Animation::getFrame(int index) {
        if(index >= (int) this->m_frames.size()) return sf::IntRect({0,0},{0,0});

        return this->m_frames[index];
    }
    int Animation::countFrames() { return this->m_frames.size(); };

    void Animation::setTexture(std::pair<std::string, sf::Texture*> texture) {
        this->m_texture.first = texture.first;
        this->m_texture.second = texture.second;
    }
    void Animation::setNextAnimation(std::string animation, int delay) {
        this->m_nextAnimation.first = animation;
        this->m_nextAnimation.second = delay;
    }
    void Animation::setDelay(int delay) { this->m_delay = delay; }
    void Animation::setCurrentFrame(int frame) { this->m_currentFrame = frame; }
    void Animation::setMode(Animation::Mode mode) { this->m_mode = mode; }

    std::pair<std::string, sf::Texture*> Animation::getTexture() { return this->m_texture; }
    std::pair<std::string, int> Animation::getNextAnimation() { return this->m_nextAnimation; }
    int Animation::getDelay() { return this->m_delay; }
    int Animation::getCurrentFrame() { return this->m_currentFrame; }
    Animation::Mode Animation::getMode() { return this->m_mode; }

    bool Animation::isPlaying() { return !this->m_stoped; }
    bool Animation::isPaused() { return this->m_stoped; }

    void Animation::play() {
        this->m_stoped = false;
        this->m_currentFrame = (this->m_currentFrame+1) % this->m_frames.size();
    }
    void Animation::pause() { this->m_stoped = true; }
    void Animation::resetFrame() {
        this->m_currentFrame = 0;
        this->m_delayStep = this->m_delay;
    }

    sf::Sprite& Animation::sprite(){ return *this->m_sprite; }
    void Animation::update(){
        if(this->m_delayStep == this->m_delay && this->m_frames.size() > 0) this->m_sprite->setTextureRect(this->m_frames[this->m_currentFrame]);

        if(this->m_delayStep <= 0 && !this->m_stoped){
            this->m_delayStep = this->m_delay;

            if(this->m_mode != Animation::Mode::PING_PONG || !this->m_pong) this->m_currentFrame++;
            else this->m_currentFrame--;

            switch(this->m_mode){
                default:
                this->m_mode = Animation::Mode::DEFAULT;
                case Animation::Mode::DEFAULT:
                this->m_currentFrame = this->m_currentFrame % this->m_frames.size();
                break;
                case Animation::Mode::PING_PONG:
                if(!this->m_pong){
                    this->m_currentFrame = this->m_currentFrame % this->m_frames.size();
                    if(this->m_currentFrame >= (int) this->m_frames.size()-1){
                        this->m_pong = true;
                    }
                }else{
                    if(this->m_currentFrame <= 0){
                        this->m_pong = false;
                    }
                }
                break;
                case Animation::Mode::STOP_ON_END:
                if(this->m_currentFrame == (int) this->m_frames.size()-1) this->pause();
                break;
            }
        }else{
            this->m_delayStep--;
        }
    }

}  // namespace game