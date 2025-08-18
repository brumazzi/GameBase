#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <scene.hpp>

namespace game{
    typedef struct __attribute__ ((__packed__)) SlotInfo{
        uint8_t     slotIndex;
        char        sceneName[128];
        uint64_t    gameTime;
        sf::Image*  image;
    }SlotInfo;

    typedef struct __attribute__ ((__packed__)) GameData GameData;

    class Game: public std::enable_shared_from_this<Game> {
        public:
        typedef std::shared_ptr<game::Game> Ptr;
        static Game::Ptr create();

        enum class SceneRenderType{
            SINGLE,
            ALL
        };

        Game();
        ~Game();

        void draw(sf::RenderWindow &render);
        void update();
        void event(std::optional<sf::Event> event);

        const std::map<std::string, game::Scene::Ptr> scenes();

        Scene::Ptr addScene(std::string name, game::Scene::Ptr scene);
        void removeScene(std::string name);
        void setScene(std::string name);
        void setRenderType(SceneRenderType type);

        game::Scene::Ptr getScene(std::string name = "");
        SceneRenderType getRenderType();

        void createSlots();
        void updateSlot(sf::Image image);
        std::vector<SlotInfo> getSlots();
        bool saveToSlot(sf::RenderWindow& window);
        bool loadFromSlot(uint lot);

        private:
        std::map<std::string, game::Scene::Ptr> m_scenes;
        std::string m_currentScene;
        SceneRenderType m_renderType;
        std::vector<SlotInfo> m_slots;
        int m_currentSlotInfo;
    };
}

#endif