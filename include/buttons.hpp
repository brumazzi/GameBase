#ifndef __BUTTONS_HPP__
#define __BUTTONS_HPP__

#include <SFML/Graphics.hpp>

namespace game {

    namespace keyboard {

        namespace buttons {
            static ::sf::Keyboard::Key CONFIRM = ::sf::Keyboard::Key::Enter;
            static ::sf::Keyboard::Key MENU = ::sf::Keyboard::Key::Escape;

            static ::sf::Keyboard::Key ACTION = ::sf::Keyboard::Key::C;
            static ::sf::Keyboard::Key CANCEL = ::sf::Keyboard::Key::X;
            static ::sf::Keyboard::Key USE = ::sf::Keyboard::Key::S;

            static ::sf::Keyboard::Key UP = ::sf::Keyboard::Key::Up;
            static ::sf::Keyboard::Key DOWN = ::sf::Keyboard::Key::Down;
            static ::sf::Keyboard::Key LEFT = ::sf::Keyboard::Key::Left;
            static ::sf::Keyboard::Key RIGHT = ::sf::Keyboard::Key::Right;
        };  // namespace buttons
    }  // namespace keyboard

    namespace joystick {
        namespace buttons {
            static uint32_t CONFIRM = 0;
            static uint32_t MENU = 0;

            static uint32_t ACTION = 0;
            static uint32_t CANCEL = 0;
            static uint32_t USE = 0;

            static uint32_t UP = 0;
            static uint32_t DOWN = 0;
            static uint32_t LEFT = 0;
            static uint32_t RIGHT = 0;
        }  // namespace buttons
    };  // namespace joystick
};  // namespace game

#endif