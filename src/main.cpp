#include <SFML/Graphics.hpp>

#include <SFML/Window/Keyboard.hpp>
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
#include <utils.hpp>
#include <settings.hpp>
#include <vars.hpp>
#include "imgui/imgui.h"

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
    window.setFramerateLimit(60);

    // std::thread t = game::resource::loadAll();
    // game::splash::show(window);
    // t.join();
    game::resource::loadAll(window);

    sf::View view(window.getView());
    view.setCenter({WINDOW_WIDTH/2.0, WINDOW_HEIGHT/2.0});
    view.zoom(((float)WINDOW_WIDTH)/window.getSize().x);
    window.setView(view);
    window.resetGLStates();
    window.setVerticalSyncEnabled(game::settings::getProperty<bool>("config.game.vsync"));

    game::vars::set<long>("system.imgui.active", ImGui::SFML::Init(window));
    if(game::vars::get<long>("system.imgui.active")){
        ImGuiIO& io = ImGui::GetIO();
        auto font = game::ui::loadConfigFont(io);
        if(font) PushFont(font);
    }

    game::physic::world::create(sf::Vector2f(0.0, 9.8));

    {
        game::Game::Ptr game = game::Game::create();
        // sf::RectangleShape shape({600,WINDOW_HEIGHT*0.75});
        // shape.setFillColor(sf::Color::Red);

        // sf::Texture waterMirrorTexture(window.getSize());
        // sf::Sprite waterMirrorSprite(waterMirrorTexture);

        // waterMirrorSprite.setScale(sf::Vector2f(1, -0.5));
        // waterMirrorSprite.setTextureRect(sf::IntRect(sf::Vector2i(0,WINDOW_HEIGHT*.25), sf::Vector2i(WINDOW_WIDTH, WINDOW_HEIGHT*.50)));
        // waterMirrorSprite.setPosition(sf::Vector2f(0, WINDOW_HEIGHT));
        // waterMirrorSprite.setColor(sf::Color(0xffffff50));

        createLevel(game);

        while(window.isOpen()){
            while(const auto event = window.pollEvent()){
                if(event->is<sf::Event::Closed>()){
                    window.close();
                }

                if(event->is<sf::Event::KeyPressed>()){
                    auto keyPressed = event->getIf<sf::Event::KeyPressed>();
                    if(keyPressed->code == sf::Keyboard::Key::A){
                        game->getScene("GameScene")->setGrid({16,16});
                        game->getScene("GameScene")->updateCollisionArea("Platform", {{0,1},{16,16}});
                        game->getScene("GameScene")->updateCollisionArea("Ground", {{1,15},{16*41,16*3}});
                    }else if(keyPressed->code == sf::Keyboard::Key::S){
                        game->getScene("GameScene")->setGrid({32,32});
                        // game->getScene("GameScene")->removeCollisionArea("Platform");
                        game->getScene("GameScene")->updateCollisionArea("Platform", {{0,1},{32,32}});
                        game->getScene("GameScene")->updateCollisionArea("Ground", {{1,15},{32*41,32*3}});
                    }else if(keyPressed->code == sf::Keyboard::Key::V){
                        game->getScene("GameScene")->removeSprite(game::scene::FAR_FOREGROUND, {0,4});
                    }
                }
            }

            game->update();

            window.clear(sf::Color(0x123456ff));

            game->draw(window);
            // waterMirrorTexture.update(window);
            // window.draw(waterMirrorSprite);

            window.display();
        }
    }
    if((game::vars::get<long>("system.imgui.active"))) PopFont();

    ImGui::SFML::Shutdown();
    game::physic::world::destroy();
    game::resource::unloadAll();

    return 0;
}

void createLevel(game::Game::Ptr game){
    game::Scene::Ptr scene = game->addScene("GameScene", game::Scene::create("default"));
    scene->setShowPhysic(true);
    // auto collision = scene->createObject("ground", "ground", sf::Vector2f(WINDOW_WIDTH/2.0, (32*3)/2.0), sf::Vector2f(20*32, 32*3));
    // collision->sprite().setTextureRect(sf::IntRect({0,0}, {0,0}));

    // auto object = scene->createObject("ground", "Ground", {1368/2, 17*32}, sf::Vector2f(684.0,32*1.5));
    scene->addCollisionArea("Platform", {{0,1},{32,32}});
    scene->addCollisionArea("Ground", {{1,15},{32*41,32*3}});
    for(int i=0; i<15; i++){
        scene->addSprite(game::scene::Layer::FAR_FOREGROUND, "ground", sf::IntRect({{32*4,32*3}, {32,32}}), {0, (float)i});
    }
    for(int i=0; i<3;i++){
        for(int j=1; j<42; j++){
            sf::Vector2f position;
            position.x = j;
            position.y = i+15;

            if(j == 1){
                scene->addSprite(game::scene::Layer::NEAR_FOREGROUND, "ground", sf::IntRect({{0,(i==2 ? 1: i)*32}, {32,32}}), position);
                // object->sprite().setTextureRect(sf::IntRect({0,(i==2 ? 1: i)*32}, {32,32}));
            }else if(j == 41){
                scene->addSprite(game::scene::Layer::NEAR_FOREGROUND, "ground", sf::IntRect({32*2,(i==2 ? 1: i)*32}, {32,32}), position);
                // object->sprite().setTextureRect(sf::IntRect({32*2,(i==2 ? 1: i)*32}, {32,32}));
            }else{
                scene->addSprite(game::scene::Layer::NEAR_FOREGROUND, "ground", sf::IntRect({32,(i==2 ? 1: i)*32}, {32,32}), position);
                // object->sprite().setTextureRect(sf::IntRect({32,(i==2 ? 1: i)*32}, {32,32}));
            }
        }
    }
}