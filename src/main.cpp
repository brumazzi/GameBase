#include <SFML/Graphics.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <cmath>
#include <physic.hpp>
#include <resource.hpp>
#include <object.hpp>
#include <scene.hpp>
#include <settings.hpp>
#include <box2d/box2d.h>
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <time.h>
#include <splash.hpp>
#include <game.hpp>
#include <vars.hpp>
#include <string>
#include <ui.hpp>
#include <utils.hpp>
#include <settings.hpp>
#include <vars.hpp>
#include <imgui/imgui.h>
#include <imgui-SFML.h>

void createLevel(game::Game::Ptr game);

int main(){
    game::settings::init("settings.cfg");
    game::translate::load();
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // BEGIN_SETTINGS_WINDOW: { // block to load settings window before game start
    //     sf::RenderWindow window(sf::VideoMode({640,480}), game::string::str_to_utf32(t("setting.window.title")), 0);
    //     window.setFramerateLimit(24);
    //     window.resetGLStates();
    //     window.setPosition(sf::Vector2i({
    //         ((int)desktop.size.x)/2-320,
    //         ((int)desktop.size.y)/2-240
    //     }));

    //     if(game::ui::settingsWindow(window)){
    //         return 0;
    //     }

    // }END_SETTINGS_WINDOW:

    game::settings::save("settings.cfg");

    game::vars::set("system.sfml.shader.active", sf::Shader::isAvailable());
    game::vars::set("system.sfml.shader_geometry.active", sf::Shader::isGeometryAvailable());

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

    sf::View view(window.getView());
    view.setCenter({WINDOW_WIDTH/2.0, WINDOW_HEIGHT/2.0});
    view.zoom(((float)WINDOW_WIDTH)/window.getSize().x);
    window.setView(view);
    window.resetGLStates();
    window.setVerticalSyncEnabled(game::settings::getProperty<bool>("config.game.vsync"));

    // Here can load assets in background or show loaded assets
    // std::thread t = game::resource::loadAll();
    // game::splash::show(window);
    // t.join();
    game::resource::loadAll(window);

    {
        game::physic::world::create(sf::Vector2f(0.0, 9.8));
        game::vars::set<long>("system.imgui.active", ImGui::SFML::Init(window));

        if(game::vars::get<long>("system.imgui.active")){
            ImGuiIO& io = ImGui::GetIO();
            auto font = game::ui::loadConfigFont(io);
            if(font) PushFont(font);
        }

        game::Game::Ptr game = game::Game::create();
        // sf::RectangleShape shape({600,WINDOW_HEIGHT*0.75});
        // shape.setFillColor(sf::Color::Red);

        // sf::Texture waterMirrorTexture(window.getSize());
        // sf::Sprite waterMirrorSprite(waterMirrorTexture);

        // waterMirrorSprite.setScale(sf::Vector2f(1, -0.5));
        // waterMirrorSprite.setTextureRect(sf::IntRect(sf::Vector2i(0,WINDOW_HEIGHT*.25), sf::Vector2i(WINDOW_WIDTH, WINDOW_HEIGHT*.50)));
        // waterMirrorSprite.setPosition(sf::Vector2f(0, WINDOW_HEIGHT));
        // waterMirrorSprite.setColor(sf::Color(0xffffff50));

        sf::Shader* shader = game::resource::shader::get("simple:grid");
        sf::RectangleShape shape;
        shape.setSize({1368,768});
        shape.setPosition({0,0});
        // sf::RenderStates state;
        // state.shader = shader;

        createLevel(game);

        sf::Clock deltaClock;
        sf::Clock clock;

        while(window.isOpen()){
            while(const auto event = window.pollEvent()){
                if(game::vars::get<long>("system.imgui.active")){
                    ImGui::SFML::ProcessEvent(window, *event);
                }

                if(event->is<sf::Event::Closed>()){
                    window.close();
                }

                if(event->is<sf::Event::KeyPressed>()){
                    auto keyPressed = event->getIf<sf::Event::KeyPressed>();
                    if(keyPressed->code == sf::Keyboard::Key::A){
                        // game->getScene("GameScene")->setGrid({16,16});
                        // game->getScene("GameScene")->updateCollisionArea("Platform", {{0,1},{16,16}});
                        // game->getScene("GameScene")->updateCollisionArea("Ground", {{1,15},{16*41,16*3}});
                        // game::physic::body::setVelocity("default", "Player", {-16.0, 0.0});
                    }else if(keyPressed->code == sf::Keyboard::Key::S){
                        // game->getScene("GameScene")->setGrid({32,32});
                        // // game->getScene("GameScene")->removeCollisionArea("Platform");
                        // game->getScene("GameScene")->updateCollisionArea("Platform", {{0,1},{32,32}});
                        // game->getScene("GameScene")->updateCollisionArea("Ground", {{1,15},{32*41,32*3}});
                        // game::physic::body::setVelocity("default", "Player", {16.0, 0.0});
                    }else if(keyPressed->code == sf::Keyboard::Key::V){
                        game->getScene("GameScene")->removeSprite(game::scene::FAR_FOREGROUND, {0,4});
                    }else if(keyPressed->code == sf::Keyboard::Key::Space){
                        // auto player = game->getScene("GameScene")->getObject("Player");
                        // b2Body_ApplyLinearImpulse(game::physic::body::get("default", "Player"), {0.0f, -600.0f}, {0,0}, true);
                        // game::physic::body::setVelocity("default", "Player", {0.0, -16.0});
                        auto bodyId = game::physic::body::get("default", "Player");
                        float mass = b2Body_GetMass(bodyId);
                        b2Body_ApplyForceToCenter(game::physic::body::get("default", "Player"), {0.0f, -mass*150}, true);

                    }
                }
            }

            if(game::vars::get<long>("system.imgui.active")){
                ImGuiIO& io = ImGui::GetIO();
                if (io.WantCaptureMouse){
                    // ImGui está usando o mouse (ex.: hover em janela) - ignore na sua app
                    // (Sua lógica de mouse da app aqui, se não capturado)
                }

                ImGui::SFML::Update(window, deltaClock.restart());
            }
            game->update();

            window.clear(sf::Color(0x123456ff));
            game::physic::world::eventHit();
            game::physic::world::eventBeginTouch();
            game::physic::world::eventEndTouch();

            shader->setUniform("u_resolution", sf::Vector2f{1368.,768.});
            shader->setUniform("u_time", clock.getElapsedTime().asSeconds());


            window.draw(shape, shader);
            game->draw(window);

            b2BodyId bodyId = game::physic::body::get("default", "Player");
            float moveSpeed = 460.0f;
            b2Vec2 point = b2Body_GetLocalCenterOfMass(bodyId);
            b2Vec2 force;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
                force = (b2Vec2){moveSpeed, 0.0f};
                float speed = b2Body_GetLinearVelocity(bodyId).x;
                speed = std::sqrt(speed*speed);
                b2Body_ApplyForceToCenter(bodyId, force, true);
                float speedY = game::physic::body::getVelocity("default", "Player").y;
                if(speed > 6.0) b2Body_SetLinearVelocity(bodyId, {6.0, speedY});
                std::cout << speed << std::endl;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
                force = (b2Vec2){-moveSpeed, 0.0f};
                float speed = b2Body_GetLinearVelocity(bodyId).x;
                speed = std::sqrt(speed*speed);
                b2Body_ApplyForceToCenter(bodyId, force, true);
                float speedY = game::physic::body::getVelocity("default", "Player").y;
                if(speed > 6.0) b2Body_SetLinearVelocity(bodyId, {-6.0, speedY});
                std::cout << speed << std::endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)){
                // b2Body_ApplyForceToCenter(game::physic::body::get("default", "Player"), {0.0f, -1200.0f}, true);
                float mass = b2Body_GetMass(bodyId);
                std::cout << mass << std::endl;
            }


            // waterMirrorTexture.update(window);
            // window.draw(waterMirrorSprite);
            if(game::vars::get<long>("system.imgui.active")) ImGui::SFML::Render(window);


            window.display();
        }
        if((game::vars::get<long>("system.imgui.active"))) PopFont();
    }

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

    auto object = scene->createObject("", "Player", {1368/2, 170}, sf::Vector2f(32, 64), game::physic::body::ShapeType::RECTANGLE, b2_dynamicBody);
    // auto object = scene->createObject("", "Player", {1368/2, 170}, sf::Vector2f(32,32), game::physic::body::ShapeType::CIRCLE, b2_dynamicBody);

    // game::physic::body::setFriction("default", object, 0.1);
    // object->addAnimation("Default", sf::IntRect({0,0},{32,32}));
    // object->setAnimation("Default");
    // object->setDelay(30);
    // auto object2 = scene->createObject("", "XPlayer", {1368/2-64, 170}, sf::Vector2f(64,64), b2_dynamicBody);
    // auto object3 = scene->createObject("", "ZPlayer", {1368/2-64, 300}, sf::Vector2f(32,32), b2_dynamicBody);
    // auto object4 = scene->createObject("", "WPlayer", {1368/2-64, 450}, sf::Vector2f(32,32), b2_dynamicBody);
    // game::physic::body::setFilterCategory("default", object4, game::physic::body::ObjectFilterType::ATTACK);

    // game::physic::body::setFilterCategory("default", object, game::physic::body::ObjectFilterType::SOLID);
    // game::physic::body::setFilterCategory("default", object2, game::physic::body::ObjectFilterType::SOLID);
    // game::physic::body::setFilterCategory("default", object3, game::physic::body::ObjectFilterType::SOLID);
    // game::physic::body::setFilterCategory("default", object4, game::physic::body::ObjectFilterType::SOLID);

    // scene->addCollisionArea("Platform", {{0,1},{32,32}});
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