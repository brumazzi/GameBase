#include <SFML/Graphics.hpp>

#include <physic.hpp>
#include <resource.hpp>
#include <object.hpp>
#include <scene.hpp>
#include <settings.hpp>
#include <time.h>
#include <splash.hpp>
#include <game.hpp>
#include <vars.hpp>
#include <string>
#include <ui.hpp>
#include "utils.hpp"

void createLevel(game::Game::Ptr game);

int main(){
    game::settings::init("settings.cfg");
    game::translate::load();
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    BEGIN_SETTINGS_WINDOW: { // block to load settings window before game start
        sf::RenderWindow window(sf::VideoMode({640,480}), game::string::str_to_utf32(t("setting.window.title")), 0);
        window.setFramerateLimit(24);
        window.resetGLStates();
        window.setPosition(sf::Vector2i({
            ((int)desktop.size.x)/2-320,
            ((int)desktop.size.y)/2-240
        }));

        if(game::ui::settingsWindow(window)){
            return 0;
        }

    }END_SETTINGS_WINDOW:

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
    view.setCenter({WINDOW_WIDTH/2.0, WINDOW_HEIGHT/2.0});
    view.zoom(((float)WINDOW_WIDTH)/window.getSize().x);
    window.setView(view);
    window.resetGLStates();
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(game::settings::getProperty<bool>("config.game.vsync"));

    // std::thread t = game::resource::loadAll();
    // game::splash::show(window);
    // t.join();
    game::resource::loadAll(window);

    game::physic::world::create(sf::Vector2f(0.0, 9.8));

    {
        game::Game::Ptr game = game::Game::create();
        // sf::RectangleShape shape({600,WINDOW_HEIGHT*0.75});
        // shape.setFillColor(sf::Color::Red);

        sf::Texture waterMirrorTexture(window.getSize());
        sf::Sprite waterMirrorSprite(waterMirrorTexture);

        waterMirrorSprite.setScale(sf::Vector2f(1, -0.5));
        waterMirrorSprite.setTextureRect(sf::IntRect(sf::Vector2i(0,WINDOW_HEIGHT*.25), sf::Vector2i(WINDOW_WIDTH, WINDOW_HEIGHT*.50)));
        waterMirrorSprite.setPosition(sf::Vector2f(0, WINDOW_HEIGHT));
        waterMirrorSprite.setColor(sf::Color(0xffffff50));

        createLevel(game);

        while(window.isOpen()){
            while(const auto event = window.pollEvent()){
                if(event->is<sf::Event::Closed>()){
                    window.close();
                }

                if(event->is<sf::Event::KeyPressed>()){
                    auto keypressed = event->getIf<sf::Event::KeyPressed>();
                }
            }

            game->update();

            window.clear(sf::Color(0x123456ff));

            game->draw(window);
            waterMirrorTexture.update(window);
            window.draw(waterMirrorSprite);

            window.display();
        }
    }

    game::physic::world::destroy();
    game::resource::unloadAll();

    return 0;
}

void createLevel(game::Game::Ptr game){
    game::Scene::Ptr scene = game->addScene("GameScene", game::Scene::create("default"));
    scene->setShowPhysic(true);
    auto collision = scene->createObject("ground", "ground", sf::Vector2f(WINDOW_WIDTH/2.0, (32*3)/2.0), sf::Vector2f(20*32, 32*3));
    collision->sprite().setTextureRect(sf::IntRect({0,0}, {0,0}));
    for(int i=0; i<3;i++){
        for(int j=0; j<43; j++){
            sf::Vector2f position;
            position.x = j*32+16;
            position.y = ((15*32)+(i*32))+16;
            auto object = scene->createObject("ground", std::string(std::to_string(j)+"-"+std::to_string(i)), position, sf::Vector2f(16,16));

            if(j == 0){
                object->sprite().setTextureRect(sf::IntRect({0,(i==2 ? 1: i)*32}, {32,32}));
            }else if(j == 42){
                object->sprite().setTextureRect(sf::IntRect({32*2,(i==2 ? 1: i)*32}, {32,32}));
            }else{
                object->sprite().setTextureRect(sf::IntRect({32,(i==2 ? 1: i)*32}, {32,32}));
            }
        }
    }
}