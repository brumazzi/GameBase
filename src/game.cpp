#include <game.hpp>

namespace game{
    Game::Game(){
        this->m_currentScene = "";
        this->m_renderType = SceneRenderType::SINGLE;
    }
    Game::~Game(){
        for(auto it: this->m_scenes){
            delete it.second;
        }
        this->m_scenes.clear();
    }

    void Game::draw(sf::RenderWindow &render){
        if(this->m_renderType == SceneRenderType::SINGLE){
            auto currentScene = this->getScene();
            if(currentScene){
                currentScene->draw(render);
            }
        }else if(this->m_renderType == SceneRenderType::ALL){
            for(auto it: this->m_scenes){
                it.second->draw(render);
            }
        }
    }
    void Game::update(){
        if(this->m_renderType == SceneRenderType::SINGLE){
            auto currentScene = this->getScene();
            if(currentScene){
                currentScene->update();
            }
        }else if(this->m_renderType == SceneRenderType::ALL){
            for(auto it: this->m_scenes){
                it.second->update();
            }
        }
    }
    void Game::event(std::optional<sf::Event> event){
        if(this->m_renderType == SceneRenderType::SINGLE){
            auto currentScene = this->getScene();
            if(currentScene){
                currentScene->event(event);
            }
        }else if(this->m_renderType == SceneRenderType::ALL){
            for(auto it: this->m_scenes){
                it.second->event(event);
            }
        }
    }

    Scene* Game::addScene(std::string name, game::Scene* scene){
        if(scene == nullptr) return nullptr;

        if(!this->m_currentScene.size()) this->m_currentScene = std::string(name);
        if(this->m_scenes.contains(name)){
            if(this->m_scenes[name] == scene) return scene;
            else delete this->m_scenes[name];
        }
        this->m_scenes[name] = scene;
        return scene;
    }
    void Game::removeScene(std::string name){
        if(this->m_scenes.contains(name)){
            this->m_scenes.erase(name);
        }
    }
    void Game::setScene(std::string name){
        if(this->m_scenes.contains(name)){
            this->m_currentScene = std::string(name);
        }
    }
    void Game::setRenderType(SceneRenderType type){
        this->m_renderType = type;
    }

    game::Scene* Game::getScene(std::string name){
        if(!name.size()) return this->m_scenes[this->m_currentScene];
        if(this->m_scenes.contains(name)) return this->m_scenes[name];
        return nullptr;
    }
    Game::SceneRenderType Game::getRenderType(){
        return this->m_renderType;
    }
}