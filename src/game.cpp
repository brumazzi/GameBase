#include <game.hpp>
#include <any>
#include <fstream>
#include <filesystem>
#include <memory>
#include <vars.hpp>
#include <cstring>
#include <settings.hpp>
#include <format>

namespace game{
    enum GameVarType{
        BOOL,
        LONG,
        DOUBLE,
        STRING,
    };
    typedef union GameVarValue{
        bool b;
        long l;
        double d;
        char s[32];
    }GameVarValue;
    typedef struct AnimationRect{
        int64_t     position[2];
        int64_t     size[2];
    }AnimationRect;
    typedef struct ObjectAnimation{
        char            name[32];
        uint64_t        rectsCount;
        // AnimationRect*  rects;
    }ObjectAnimation;
    typedef struct GameVar{
        char            name[32];
        GameVarType     type;
        GameVarValue    value;
    }GameVar;
    typedef struct __attribute__ ((__packed__)) ObjectInfo{
        char                currentAnimation[32];
        char                textureName[64];
        uint64_t            animationsCount;
        bool                drawable;
        uint16_t            animationDelay;

        bool                usePhysics;
        char                worldName[32];
        char                bodyName[32];
        uint                bodyType;
        float               bodyDensity;
        bool                bodyFixRotation;
        float               bodyFriction;
        float               bodyRestitution;
        float               position[2];
        float               velocity[2];
        float               size[2];

        // ObjectAnimation*    animations;
    }ObjectInfo;
    typedef struct __attribute__ ((__packed__)) SceneInfo{
        char            sceneName[128];
        char            worldName[32];
        uint64_t        varsCount;
        uint64_t        objectsCount;
        bool            drawCollisionLines;
        double          worldGravity[2];

        // GameVar*        vars;
        // ObjectInfo*     objects;
    }SceneInfo;

    typedef struct __attribute__ ((__packed__)) GameData{
        char            currentScene[32];
        uint64_t        globalVarsCount;
        uint64_t        scenesCount;

        // GameVar*        globalVars;
        // SceneInfo*      scenes;
    }GameData;

    Game::Ptr Game::create(){ return std::make_shared<game::Game>(); }

    Game::Game(){
        this->m_currentScene = "";
        this->m_renderType = SceneRenderType::SINGLE;
        this->m_currentSlotInfo = 0;
    }
    Game::~Game(){
        for(auto slot: this->m_slots){
            delete slot.image;
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

    const std::map<std::string, game::Scene::Ptr> Game::scenes(){
        return this->m_scenes;
    }

    Scene::Ptr Game::addScene(std::string name, game::Scene::Ptr scene){
        if(scene == nullptr) return nullptr;

        if(!this->m_currentScene.size()) this->m_currentScene = std::string(name);
        if(this->m_scenes.contains(name)){
            if(this->m_scenes[name] == scene) return scene;
            else this->m_scenes[name].reset();
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

    game::Scene::Ptr Game::getScene(std::string name){
        if(!name.size()) return this->m_scenes[this->m_currentScene];
        if(this->m_scenes.contains(name)) return this->m_scenes[name];
        return nullptr;
    }
    Game::SceneRenderType Game::getRenderType(){
        return this->m_renderType;
    }

    void Game::createSlots(){
        SlotInfo slot;
        char *cslot = (char*) &slot;
        for(int i=0; i<sizeof(SlotInfo); i++) cslot[i] = 0;

        this->m_slots.emplace_back(slot);
        this->m_currentSlotInfo = this->m_slots.size()-1;
    }
    void Game::updateSlot(sf::Image image){
        float incW=1, incH=1;
        sf::Image newImage({256, 256});
        incW = (float) image.getSize().x/256.0;
        incH = (float) image.getSize().y/256.0;

        for(uint w=0; w<256; w++){
            for(uint h=0; h<256; h++){
                newImage.setPixel({w,h}, image.getPixel({w*incW, h*incH}));
            }
        }

        strcpy(this->m_slots[this->m_currentSlotInfo].sceneName, this->m_currentScene.c_str());
        this->m_slots[this->m_currentSlotInfo].slotIndex = this->m_currentSlotInfo-1;
        // this->m_slots[this->m_currentSlotInfo].gameTime = ; // TODO: implement game play time

        std::string savePath = game::settings::getHomeDir();
        #ifdef __WIN32
        savePath.append("/game");
        #else
        savePath.append("/.game");
        #endif
        std::string savePathImage(savePath);

        std::filesystem::path dir(savePath);
        if (!std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }
        savePath.append("/data.slt");
        savePathImage.append("/shot_"+std::format("{:02}", this->m_currentSlotInfo)+".png");

        std::ofstream output(savePath);
        for(const auto slot: this->m_slots){
            output.write(reinterpret_cast<const char*>(&slot), sizeof(SlotInfo));
        }
        newImage.saveToFile(savePathImage);
        output.close();
    }

    std::vector<SlotInfo> Game::getSlots(){
        std::string loadPath = game::settings::getHomeDir();
        #ifdef __WIN32
        loadPath.append("/game");
        #else
        loadPath.append("/.game");
        #endif

        std::filesystem::path dir(loadPath);
        if (!std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }

        loadPath.append("/data.slt");
        std::ifstream input(loadPath);
        if(input.is_open()){
            unsigned int slotsCount = std::filesystem::file_size(loadPath)/sizeof(SlotInfo);
            while(slotsCount--){
                SlotInfo slotInfo;
                input.read(reinterpret_cast<char*>(&slotInfo), sizeof(SlotInfo));
                slotInfo.image = new sf::Image({256, 256});

                this->m_slots.emplace_back(slotInfo);

                std::string loadPathImage(dir.generic_string());
                loadPathImage.append("/shot_"+std::format("{:02}", this->m_currentSlotInfo)+".png");
                slotInfo.image->loadFromFile(loadPathImage);
                this->m_slots[this->m_slots.size()-1].image->loadFromFile(std::string(loadPathImage));
            }
        }

        return this->m_slots;
    }
    bool Game::saveToSlot(sf::RenderWindow& window){
        std::string savePath = game::settings::getHomeDir();
        #ifdef __WIN32
        savePath.append("/game/");
        #else
        savePath.append("/.game/");
        #endif

        std::filesystem::path dir(savePath);
        if (!std::filesystem::exists(dir)) {
            std::filesystem::create_directory(dir);
        }
        char fileName[16];
        sprintf(fileName, "/slot_%.2d.bin", this->m_currentSlotInfo);
        savePath.append(fileName);

        std::ofstream output(savePath);

        // count global vars and scenes in game
        GameData gameData;
        std::vector<std::string> varList;
        game::vars::list(varList);

        std::strcpy(gameData.currentScene, this->m_currentScene.c_str());
        gameData.globalVarsCount = varList.size();
        gameData.scenesCount = this->m_scenes.size();
        output.write(reinterpret_cast<const char*>(&gameData), sizeof(GameData));

        for(auto var: varList){
            GameVar gameVar;
            std::strcpy(gameVar.name, var.c_str());
            if(game::vars::is<long>(var)){
                gameVar.type = GameVarType::LONG;
                gameVar.value.l = game::vars::get<long>(var);
            }else if(game::vars::is<double>(var)){
                gameVar.type = GameVarType::DOUBLE;
                gameVar.value.d = game::vars::get<double>(var);
            }else if(game::vars::is<bool>(var)){
                gameVar.type = GameVarType::BOOL;
                gameVar.value.b = game::vars::get<bool>(var);
            }else if(game::vars::is<std::string>(var)){
                gameVar.type = GameVarType::STRING;
                std::strcpy(gameVar.value.s, game::vars::get<std::string>(var).c_str());
            }
            output.write(reinterpret_cast<const char*>(&gameVar), sizeof(GameVar));
        }

        uint64_t sceneIndex = 0;
        for(auto it: this->m_scenes){
            SceneInfo sceneInfo;

            std::strcpy(sceneInfo.sceneName, it.first.c_str());
            std::strcpy(sceneInfo.worldName, it.second->getWorld().c_str());
            std::vector<std::string> sceneVarList;
            sf::Vector2f worldGravity = game::physic::world::getGravity(it.second->getWorld());

            it.second->varList(sceneVarList);

            sceneInfo.worldGravity[0] = worldGravity.x;
            sceneInfo.worldGravity[1] = worldGravity.y;
            sceneInfo.varsCount = sceneVarList.size();
            sceneInfo.objectsCount = it.second->objects().size();

            output.write(reinterpret_cast<const char*>(&sceneInfo), sizeof(SceneInfo));

            for(auto var: sceneVarList){
                GameVar gameVar;
                std::strcpy(gameVar.name, var.c_str());
                if(it.second->varIs<long>(var)){
                    gameVar.type = GameVarType::LONG;
                    gameVar.value.l = it.second->getVar<long>(var);
                }else if(it.second->varIs<double>(var)){
                    gameVar.type = GameVarType::DOUBLE;
                    gameVar.value.d = it.second->getVar<double>(var);
                }else if(it.second->varIs<bool>(var)){
                    gameVar.type = GameVarType::BOOL;
                    gameVar.value.b = it.second->getVar<bool>(var);
                }else if(it.second->varIs<std::string>(var)){
                    gameVar.type = GameVarType::STRING;
                    std::strcpy(gameVar.value.s, it.second->getVar<std::string>(var).c_str());
                }else return false;
                output.write(reinterpret_cast<const char*>(&gameVar), sizeof(GameVar));
            }

            for(auto objIt: it.second->objects()){
                auto object = objIt.second;

                ObjectInfo objectInfo;
                std::strcpy(objectInfo.currentAnimation, object->getAnimation().c_str());
                std::strcpy(objectInfo.textureName, object->getTextureName().c_str());
                objectInfo.animationsCount = object->animations().size();

                objectInfo.usePhysics = object->getBodyName().empty() ? false : true;
                std::strcpy(objectInfo.worldName, object->getWorldName().c_str());
                std::strcpy(objectInfo.bodyName, object->getBodyName().c_str());
                // objectInfo.drawable = ;

                objectInfo.bodyType = game::physic::body::getType(object->getWorldName(), object);
                objectInfo.bodyDensity = game::physic::body::getDensity(object->getWorldName(), object);
                objectInfo.bodyFixRotation = game::physic::body::getFixRotation(object->getWorldName(), object);
                objectInfo.bodyFriction = game::physic::body::getFriction(object->getWorldName(), object);
                objectInfo.bodyRestitution = game::physic::body::getRestitution(object->getWorldName(), object);
                sf::Vector2f position = game::physic::body::getPosition(object->getWorldName(), object),
                             velocity = game::physic::body::getVelocity(object->getWorldName(), object),
                             size = object->getSize();
                objectInfo.position[0] = position.x;
                objectInfo.position[1] = position.y;
                objectInfo.velocity[0] = velocity.x;
                objectInfo.velocity[1] = velocity.y;
                objectInfo.size[0] = size.x;
                objectInfo.size[1] = size.y;
                objectInfo.animationDelay = object->getDelay();

                output.write(reinterpret_cast<const char*>(&objectInfo), sizeof(ObjectInfo));

                for(auto aniIt: object->animations()){
                    ObjectAnimation objectAnimation;

                    std::strcpy(objectAnimation.name, aniIt.first.c_str());
                    objectAnimation.rectsCount = aniIt.second.size();
                    output.write(reinterpret_cast<const char*>(&objectAnimation), sizeof(ObjectAnimation));

                    for(auto rect: aniIt.second){
                        output.write(reinterpret_cast<const char*>(&rect.position.x), sizeof(int));
                        output.write(reinterpret_cast<const char*>(&rect.position.y), sizeof(int));
                        output.write(reinterpret_cast<const char*>(&rect.size.x), sizeof(int));
                        output.write(reinterpret_cast<const char*>(&rect.size.y), sizeof(int));
                    }
                }
            }
        }

        output.close();
        sf::Texture tex(window.getSize());
        tex.update(window);
        this->updateSlot(tex.copyToImage());
        return true;
    }
    bool Game::loadFromSlot(uint slot){
        std::string savePath = game::settings::getHomeDir();
        #ifdef __WIN32
        savePath.append("/game/");
        #else
        savePath.append("/.game/");
        #endif

        std::filesystem::path dir(savePath);
        if (!std::filesystem::exists(dir)) {
            std::filesystem::create_directory(dir);
        }
        char fileName[16];
        sprintf(fileName, "/slot_%.2d.bin", slot);
        savePath.append(fileName);

        std::ifstream input(savePath);

        GameData gameData;
        input.read(reinterpret_cast<char*>(&gameData), sizeof(GameData));

        while(gameData.globalVarsCount--){
            GameVar gameVar;
            input.read(reinterpret_cast<char*>(&gameVar), sizeof(GameVar));

            if(gameVar.type == GameVarType::LONG){
                game::vars::set<long>(std::string(gameVar.name), gameVar.value.l);
            }else if(gameVar.type == GameVarType::DOUBLE){
                game::vars::set<double>(std::string(gameVar.name), gameVar.value.d);
            }else if(gameVar.type == GameVarType::BOOL){
                game::vars::set<bool>(std::string(gameVar.name), gameVar.value.b);
            }else if(gameVar.type == GameVarType::STRING){
                game::vars::set<std::string>(std::string(gameVar.name), std::string(gameVar.value.s));
            }else return false;
        }

        while(gameData.scenesCount--){
            SceneInfo sceneInfo;
            input.read(reinterpret_cast<char*>(&sceneInfo), sizeof(SceneInfo));

            if(!game::physic::world::exists(sceneInfo.worldName)){
                game::physic::world::create(sceneInfo.worldGravity[0], sceneInfo.worldGravity[1], sceneInfo.worldName);
            }

            auto scene = this->addScene(std::string(sceneInfo.sceneName), Scene::create(sceneInfo.worldName));
            while(sceneInfo.varsCount--){
                GameVar gameVar;
                input.read(reinterpret_cast<char*>(&gameVar), sizeof(GameVar));
                if(gameVar.type == GameVarType::LONG){
                    scene->setVar<long>(std::string(gameVar.name), gameVar.value.l);
                }else if(gameVar.type == GameVarType::DOUBLE){
                    scene->setVar<double>(std::string(gameVar.name), gameVar.value.d);
                }else if(gameVar.type == GameVarType::BOOL){
                    scene->setVar<bool>(std::string(gameVar.name), gameVar.value.b);
                }else if(gameVar.type == GameVarType::STRING){
                    scene->setVar<std::string>(std::string(gameVar.name), std::string(gameVar.value.s));
                }else return false;
            }

            while(sceneInfo.objectsCount--){
                ObjectInfo objectInfo;
                input.read(reinterpret_cast<char*>(&objectInfo), sizeof(ObjectInfo));

                auto object = scene->createObject(
                    std::string(objectInfo.textureName),
                    objectInfo.bodyName,
                    {objectInfo.position[0], objectInfo.position[1]},
                    {objectInfo.size[0], objectInfo.size[1]},
                    (b2BodyType)objectInfo.bodyType
                );
                object->setDelay(objectInfo.animationDelay);

                if(objectInfo.usePhysics){
                    sf::Vector2f position = sf::Vector2f(objectInfo.position[0], objectInfo.position[1]);
                    sf::Vector2f velocity = sf::Vector2f(objectInfo.velocity[0], objectInfo.velocity[1]);
                    sf::Vector2f size = sf::Vector2f(objectInfo.size[0], objectInfo.size[1]);
                    game::physic::body::setDensity(objectInfo.worldName, object, objectInfo.bodyDensity);
                    game::physic::body::setFriction(objectInfo.worldName, object, objectInfo.bodyFriction);
                    game::physic::body::setRestitution(objectInfo.worldName, object, objectInfo.bodyRestitution);
                    game::physic::body::setFixRotation(objectInfo.worldName, object, objectInfo.bodyFixRotation);
                }

                while(objectInfo.animationsCount--){
                    ObjectAnimation animation;
                    input.read(reinterpret_cast<char*>(&animation), sizeof(ObjectAnimation));

                    while(animation.rectsCount--){
                        int intVec[4];
                        input.read(reinterpret_cast<char*>(intVec), sizeof(int)*4);
                        object->addAnimation(animation.name, sf::IntRect({intVec[0],intVec[1]}, {intVec[2],intVec[3]}));
                    }
                }
                object->setAnimation(objectInfo.currentAnimation);
            }
        }

        this->m_currentSlotInfo = slot;
        return true;
    }
}