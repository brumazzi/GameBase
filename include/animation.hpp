#ifndef __ANIMATION_HPP__
#define __ANIMATION_HPP__

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <utility>
#include <vector>

namespace game{
    class Animation: public std::enable_shared_from_this<Animation>{
        public:

        enum class Mode{
            DEFAULT,
            PING_PONG,
            STOP_ON_END
        };

        typedef std::shared_ptr<game::Animation> Ptr;
        static Animation::Ptr create(std::pair<std::string, sf::Texture*> texture);

        Animation(std::pair<std::string, sf::Texture*> texture);
        ~Animation();

        void addFrame(sf::IntRect rect);
        void updateFrame(int index, sf::IntRect rect);
        void removeFrame(int index);
        sf::IntRect getFrame(int index);
        int countFrames();

        void setTexture(std::pair<std::string, sf::Texture*> texture);
        void setNextAnimation(std::string animation, int delay);
        void setDelay(int delay);
        void setCurrentFrame(int frame);
        void setMode(Animation::Mode mode);

        std::pair<std::string, sf::Texture*> getTexture();
        std::pair<std::string, int> getNextAnimation();
        int getDelay();
        int getCurrentFrame();
        Animation::Mode getMode();

        bool isPlaying();
        bool isPaused();

        void play();
        void pause();
        void resetFrame();

        sf::Sprite &sprite();
        void update();

        private:
        std::pair<std::string, sf::Texture*> m_texture;
        sf::Sprite* m_sprite;
        std::vector<sf::IntRect> m_frames;
        std::pair<std::string, int> m_nextAnimation;
        int m_delay;
        int m_delayStep;
        int m_currentFrame;

        Animation::Mode m_mode;
        bool m_pong;
        bool m_stoped;
    };
}

#endif