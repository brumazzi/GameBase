#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <XMLLoader/XMLLoader.hpp>

#include <physic.hpp>
#include <resource.hpp>
#include <object.hpp>
#include <scene.hpp>
#include <settings.hpp>
#include <time.h>
#include <splash.hpp>
#include <game.hpp>

int main(){
    game::settings::init("settings.cfg");
    game::translate::load();
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    BEGIN_SETTINGS_WINDOW:
    { // block to load settings window before game start
        bool endGame = false;
        sf::RenderWindow window(sf::VideoMode({640,480}), "Settings", 0);
        window.setFramerateLimit(60);
        window.resetGLStates();
        window.setPosition(sf::Vector2i({
            ((int)desktop.size.x)/2-320,
            ((int)desktop.size.y)/2-240
        }));

        if(game::settings::openSettingWindow(window)) return 0;
    }
    END_SETTINGS_WINDOW:

    game::settings::save("settings.cfg");

    sf::RenderWindow window(
        sf::VideoMode({
            game::settings::getProperty<unsigned int>("config.style.window.width"),
            game::settings::getProperty<unsigned int>("config.style.window.height")
        }),
        "Game",
        sf::Style::Titlebar,
        game::settings::getProperty<bool>("config.style.window.fullscreen") ? sf::State::Fullscreen : sf::State::Windowed
    );
    sf::View view(window.getView());
    view.setCenter({WINDOW_WIDTH/2, WINDOW_HEIGHT/2});
    view.zoom(((float)WINDOW_WIDTH)/window.getSize().x);
    window.setView(view);
    window.resetGLStates();
    window.setFramerateLimit(60);

    // std::thread t = game::resource::loadAll();
    // game::splash::show(window);
    // t.join();
    game::resource::loadAll(window);

    game::physic::world::create(sf::Vector2f(0.0, 9.8));
    auto texture = game::resource::texture::loadFromFile("res", "wolf-run.png");

    {
        game::Game game;
        auto *scene = game.addScene("Primary", new game::Scene("default"));


        // auto object = scene->createObject("res", "Object", sf::Vector2f(100, 16), sf::Vector2f((16*5)/2, (16*5)/2), b2_staticBody);
        auto object = scene->createObject("res", "Object", sf::Vector2f(100, 16), sf::Vector2f((16*5)/2, (16*5)/2), b2_dynamicBody);

        object->addAnimation("idle", sf::IntRect({64*0, 0},{64, 64}));
        object->addAnimation("idle", sf::IntRect({64*1, 0},{64, 64}));
        object->addAnimation("idle", sf::IntRect({64*2, 0},{64, 64}));
        object->addAnimation("idle", sf::IntRect({64*3, 0},{64, 64}));
        object->addAnimation("idle", sf::IntRect({64*4, 0},{64, 64}));
        object->addAnimation("idle", sf::IntRect({64*5, 0},{64, 64}));
        object->addAnimation("idle", sf::IntRect({64*6, 0},{64, 64}));
        object->addAnimation("idle", sf::IntRect({64*7, 0},{64, 64}));
        object->setAnimation("idle");
        object->setDelay(5);
        object->sprite().scale({5, 5});

        game::physic::body::setRestitution("default", object, 0.0);

        auto windowSize = window.getSize();
        auto ground = scene->createObject("", "Ground", sf::Vector2f(320, windowSize.y/2), sf::Vector2f(300, 16));

        sf::Texture texture(windowSize);
        windowSize.y /= 2;
        sf::Sprite sprite(texture, sf::IntRect({0,0}, {windowSize.x, windowSize.y}));
        sprite.setScale({1, -1});
        sprite.setPosition({0, window.getSize().y});

        while(window.isOpen()){
            while(const auto event = window.pollEvent()){
                if(event->is<sf::Event::Closed>()){
                    window.close();
                }

                if(event->is<sf::Event::KeyPressed>()){
                    auto keypressed = event->getIf<sf::Event::KeyPressed>();
                }
            }

            game.update();

            window.clear(sf::Color(0x123456ff));

            game.draw(window);

            texture.update(window);
            window.draw(sprite);
            window.display();
        }
    }

    game::physic::world::destroy();
    game::resource::unloadAll();
    return 0;
}