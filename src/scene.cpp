#include <scene.hpp>
#include <resource.hpp>

namespace game {
    Scene::Ptr Scene::create(std::string world){ return std::make_shared<game::Scene>(world); }

    Scene::Scene(std::string world){
        this->m_world = world;
    }
    Scene::~Scene(){
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

    const std::map<std::string, game::Object::Ptr> Scene::objects(){
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