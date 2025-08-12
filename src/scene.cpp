#include <scene.hpp>
#include <resource.hpp>

namespace game {
    Scene::Scene(std::string world){
        this->m_world = world;
    }
    Scene::~Scene(){
        for(auto it: this->m_objects){
            if(game::physic::world::exists(this->m_world)){
                game::physic::body::destroy(this->m_world, it.second->getBodyName());
            }
            delete it.second;
        }

        this->m_objects.clear();
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
        for(auto it: this->m_objects){
            if(it.second->isDestroyed() || !it.second->isDrawable()) continue;
            render.draw(*it.second);
        }
        if(this->m_showPhysic) game::physic::world::drawLines(render, this->m_world);
    }
    void Scene::event(std::optional<sf::Event> event){
        // TODO: do this
    }

    void Scene::setWorld(std::string world){ this->m_world = world; }
    void Scene::setShowPhysic(bool flag){ this->m_showPhysic = flag; }
    void Scene::setName(std::string name){ this->m_name = name; }
    std::string Scene::getWorld(){ return this->m_world; }
    bool Scene::getShowPhysic(){ return this->m_showPhysic; }
    std::string Scene::getName(){ return this->m_name; }

    bool Scene::addObject(game::Object* object, std::string name){
        if(!object) return false;
        if(!name.size()){
            name = object->getBodyName();
            if(!name.size()) return false;
        }

        if(this->m_objects.contains(name)) return false;

        this->m_objects[name] = object;
        return true;
    }
    game::Object* Scene::createObject(std::string texture, std::string name, sf::Vector2f pos, sf::Vector2f size, b2BodyType type){
        game::Object* newObject;

        if(texture.size()) newObject = new game::Object(texture);
        else newObject = new game::Object();

        if(game::physic::world::exists(this->m_world)){
            game::physic::body::create(this->m_world, name, newObject, pos, size, type);
        }
        this->addObject(newObject, name);

        return newObject;
    }

    void Scene::removeObject(std::string object){
        if(!this->m_objects.contains(object)) return;

        game::Object* delObject = this->m_objects[object];
        this->m_objects.erase(object);
        if(game::physic::world::exists(this->m_world)){
            game::physic::body::destroy(this->m_world, delObject->getWorldName());
        }
        delete delObject;
    }
    void Scene::removeObject(game::Object* object){
        this->removeObject(object->getBodyName());
    }

    game::Object* Scene::getObject(std::string object){
        if(this->m_objects.contains(object)) return this->m_objects[object];
        return nullptr;
    }

    template <typename T> T Scene::getVar(std::string key){
        return this->m_vars[key].as<T>();
    }
    template <typename T> void Scene::setVar(std::string key, T value){
        this->m_vars[key] = value;
    }

    template std::string Scene::getVar(std::string key);
    template int Scene::getVar(std::string key);
    template double Scene::getVar(std::string key);
    template bool Scene::getVar(std::string key);
    template void Scene::setVar(std::string key, std::string value);
    template void Scene::setVar(std::string key, int value);
    template void Scene::setVar(std::string key, double value);
    template void Scene::setVar(std::string key, bool value);
}