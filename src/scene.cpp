#include <box2d/types.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <scene.hpp>
#include <resource.hpp>
#include <utility>
#include <vector>
#include "physic.hpp"

namespace game {
    Scene::Ptr Scene::create(std::string world){ return std::make_shared<game::Scene>(world); }

    Scene::Scene(std::string world){
        this->m_world = world;
        this->m_grid = sf::Vector2u{32,32};
    }
    Scene::~Scene(){
        this->m_objects.clear();
        for(auto it: this->m_layers){
            for(auto it2: it.second){
                for(auto it3: it2.second){
                    delete it3.second;
                }
            }
        }
        this->m_layers.clear();
        this->m_collisionArea.clear();
    }

    void Scene::update(){
        if(game::physic::world::exists(this->m_world)){
            game::physic::world::update(this->m_world);
        }

        std::vector<std::string> destroyedObjects;
        for(auto it: this->m_objects){
            if(it.second->isDestroyed()){
                destroyedObjects.push_back(it.first);
                continue;
            }
            it.second->update();
        }

        for(std::string objName: destroyedObjects){
            this->removeObject(objName);
        }
    }
    void Scene::draw(sf::RenderWindow& render){
        sf::FloatRect viewport(render.getView().getViewport());

        viewport.size.x = (int)(viewport.size.x*render.getSize().x/this->m_grid.x);
        viewport.size.y = (int)(viewport.size.y*render.getSize().y/this->m_grid.y);
        viewport.position.x = (int)(viewport.position.x/this->m_grid.x);
        viewport.position.y = (int)(viewport.position.y/this->m_grid.y);

        for(int i=0; i<game::scene::Layer::LEVEL_COUNT; i++){
            for(auto itX: this->m_layers[(scene::Layer)i]){
                if(itX.first >= (viewport.position.x-2) && itX.first <= (viewport.size.x+2)){
                    for(auto itY: itX.second){
                        if(itY.first >= (viewport.position.y) && itY.first <= (viewport.size.y)){
                            sf::Sprite* sprite = itY.second;
                            render.draw(*sprite);
                        }
                    }
                }
            }

            if(i == game::scene::Layer::OBJECT_FIELD){
                for(auto it: this->m_objects){
                    if(it.second->isDestroyed() || !it.second->isDrawable()) continue;
                    render.draw(*it.second);
                }
            }
        }
        if(this->m_showPhysic) game::physic::world::drawLines(render, this->m_world);
    }
    void Scene::event(std::optional<sf::Event> event){
        // TODO: do this
    }

    void Scene::setWorld(std::string world){ this->m_world = world; }
    void Scene::setShowPhysic(bool flag){ this->m_showPhysic = flag; }
    void Scene::setName(std::string name){ this->m_name = name; }
    void Scene::setGrid(sf::Vector2u grid){
        for(auto [layer, data]: this->m_layers){
            for(auto [x, data2]: data){
                for(auto [y, sprite]: data2){
                    sf::IntRect newRect(sprite->getTextureRect());
                    newRect.position.x = newRect.position.x/this->m_grid.x*grid.x;
                    newRect.position.y = newRect.position.y/this->m_grid.y*grid.y;
                    newRect.size.x = grid.x;
                    newRect.size.y = grid.y;
                    sprite->setTextureRect(newRect);

                    sf::Vector2f newPos;
                    newPos.x = x*grid.x;
                    newPos.y = y*grid.y;
                    sprite->setPosition(newPos);
                }
            }
        }
        this->m_grid = grid;
    }
    std::string Scene::getWorld(){ return this->m_world; }
    bool Scene::getShowPhysic(){ return this->m_showPhysic; }
    std::string Scene::getName(){ return this->m_name; }
    sf::Vector2u Scene::getGrid(){ return this->m_grid; }

    const std::unordered_map<std::string, game::Object::Ptr> Scene::objects(){
        return this->m_objects;
    }
    void Scene::objectList(std::vector<std::string>& list){
        for(auto it: this->m_objects){
            list.emplace_back(it.first);
        }
    }
    bool Scene::addObject(game::Object::Ptr object, std::string name){
        if(!object) return false;
        if(!name.size()){
            name = object->getBodyName();
            if(!name.size()) return false;
        }

        if(this->m_objects.contains(name)) return false;

        this->m_objects[name] = object;
        return true;
    }
    game::Object::Ptr Scene::createObject(std::string texture, std::string name, sf::Vector2f pos, sf::Vector2f size, b2BodyType type){
        game::Object::Ptr newObject;

        if(texture.size()) newObject = game::Object::create(texture);
        else newObject = game::Object::create();

        if(game::physic::world::exists(this->m_world)){
            game::physic::body::create(this->m_world, name, newObject, pos, size, type);
        }
        this->addObject(newObject, name);
        newObject->setSize(size);

        return newObject;
    }
    void Scene::addSprite(game::scene::Layer layerID, std::string texture, sf::IntRect rect, sf::Vector2f pos){
        sf::Sprite* sprite = new sf::Sprite(*game::resource::texture::get(texture));
        sprite->setTextureRect(rect);
        sprite->setPosition({pos.x*this->m_grid.x, pos.y*this->m_grid.y});
        this->m_layers[layerID][pos.x][pos.y] = sprite;
    }
    void Scene::removeSprite(game::scene::Layer layerID, sf::Vector2f pos){
        this->m_layers[layerID][pos.x].erase(pos.y);
    }
    void Scene::addCollisionArea(std::string name, sf::FloatRect rect){
        if(this->m_collisionArea.contains(name)) return;

        rect.position.x = rect.position.x*this->m_grid.x + (rect.size.x/2.0);
        rect.position.y = rect.position.y*this->m_grid.y + (rect.size.y/2.0);
        rect.size.x = rect.size.x/2;
        rect.size.y = rect.size.y/2;

        this->m_collisionArea.emplace(name, rect);
        game::physic::body::create(this->m_world, name, nullptr, rect.position, rect.size);
    }
    void Scene::updateCollisionArea(std::string name, sf::FloatRect rect){
        this->removeCollisionArea(name);
        this->addCollisionArea(name, rect);
    }
    void Scene::removeCollisionArea(std::string name){
        if(!this->m_collisionArea.contains(name)) return;

        this->m_collisionArea.erase(name);
        game::physic::body::destroy(this->m_world, name);
    }

    void Scene::removeObject(std::string object){
        if(!this->m_objects.contains(object)) return;

        if(game::physic::world::exists(this->m_world)){
            game::physic::body::destroy(this->m_world, this->m_objects[object]->getWorldName());
        }
        this->m_objects[object].reset();
        this->m_objects.erase(object);
    }
    void Scene::removeObject(game::Object::Ptr object){
        this->removeObject(object->getBodyName());
    }

    game::Object::Ptr Scene::getObject(std::string object){
        if(this->m_objects.contains(object)) return this->m_objects[object];
        return nullptr;
    }

    void Scene::varList(std::vector<std::string>& vector){
        for(auto it: this->m_vars){
            vector.emplace_back(it.first);
        }
    }
    std::string Scene::varType(std::string key){
        return std::string(this->m_vars[key].type().name());
    }
    template<typename T> bool Scene::varIs(std::string key){
        return this->m_vars[key].type() == typeid(T);
    }
    template <typename T> T Scene::getVar(std::string key){
        return std::any_cast<T>(this->m_vars[key]);
    }
    template <typename T> void Scene::setVar(std::string key, T value){
        this->m_vars[key] = value;
    }

    template std::string Scene::getVar(std::string key);
    template long Scene::getVar(std::string key);
    template double Scene::getVar(std::string key);
    template bool Scene::getVar(std::string key);
    template void Scene::setVar(std::string key, std::string value);
    template void Scene::setVar(std::string key, long value);
    template void Scene::setVar(std::string key, double value);
    template void Scene::setVar(std::string key, bool value);

    template bool Scene::varIs<std::string>(std::string key);
    template bool Scene::varIs<long>(std::string key);
    template bool Scene::varIs<double>(std::string key);
    template bool Scene::varIs<bool>(std::string key);
}