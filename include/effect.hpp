#ifndef __EFFECT_HPP__
#define __EFFECT_HPP__

#include <SFML/Graphics.hpp>

namespace game {
    namespace effect {
        enum class Type { FOG_IN, FOG_OUT, SHAKE };

        void fogIn(::sf::Color color, float seconds);
        void fogOut(float seconds);
        void shake(::sf::View *view, float intensity, float seconds);

        void render(::sf::RenderWindow &render);
        bool isLocked();
        void waitFor();
    }  // namespace effect

    class Effect {
        public:
        effect::Type type;
        ::sf::View *view;
        ::sf::Color color;
        float seconds;
        float intensity;

        Effect(::sf::View *view, ::sf::Color color, float seconds,
               float intensity, effect::Type type);
    };
}  // namespace game

#endif