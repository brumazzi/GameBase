#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <splash.hpp>
#include <iostream>
#include <vector>
#include <resource.hpp>
#include <settings.hpp>

INCBIN(SPLASH_01, "./src/resources/splash/logo1.png")
INCBIN(SPLASH_02, "./src/resources/splash/logo2.png")
INCBIN(SPLASH_03, "./src/resources/splash/logo3.png")

#define dt 256/60.0

namespace game{
    namespace splash{
        void show(sf::RenderWindow &render){
            std::vector<sf::Sprite> splashes;

            // buind splashes
            sf::Texture tex_01;
            if(tex_01.loadFromMemory(SPLASH_01, SPLASH_01_size))
                splashes.emplace_back(tex_01);
            sf::Texture tex_02;
            if(tex_02.loadFromMemory(SPLASH_02, SPLASH_02_size))
                splashes.emplace_back(tex_02);
            sf::Texture tex_03;
            if(tex_03.loadFromMemory(SPLASH_03, SPLASH_03_size))
                splashes.emplace_back(tex_03);

            std::string error;

            for(auto sprite: splashes){
                int alpha = 1;
                bool invert = false;
                int delay = 3*60;

                while(alpha > 0.0){
                    while(std::optional event = render.pollEvent()){
                        if(event->is<sf::Event::KeyPressed>()){
                            auto keyboardEvent = event->getIf<sf::Event::KeyPressed>();
                            if(keyboardEvent->code == sf::Keyboard::Key::Escape || keyboardEvent->code == sf::Keyboard::Key::Enter) delay = 0;
                        }
                    }

                    if(!invert) alpha += dt;
                    else{
                        if(delay > 0) delay--;
                        else alpha -= dt;
                    }

                    if(alpha >= 256){
                        invert = true;
                        alpha = 255;
                    }

                    render.clear(sf::Color(0xff));

                    sprite.setColor(sf::Color((uint32_t)(0xffffff00 + ((int) alpha))));

                    render.draw(sprite);

                    error = game::resource::getError();
                    if(error.size()){
                        sf::Font font;
                        if(!font.openFromFile(game::settings::getProperty<std::string>("config.style.font"))){
                            render.close();
                            return;
                        }
                        sf::Text msg(font, error, 16);

                        msg.setFillColor(sf::Color(0xaf0000ff));
                        msg.setPosition({10, WINDOW_HEIGHT-26.f});

                        render.draw(msg);
                        render.display();
                        sf::sleep(sf::seconds(4));
                        render.close();
                        break;
                    }

                    render.display();
                }
            }
        }
    }
}