#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <scene.hpp>

namespace game{
    class Game {
        public:
        enum class SceneRenderType{
            SINGLE,
            ALL
        };

        Game();
        ~Game();

        void draw(sf::RenderWindow &render);
        void update();
        void event(std::optional<sf::Event> event);

        Scene* addScene(std::string name, game::Scene* scene = nullptr);
        void removeScene(std::string name);
        void setScene(std::string name);
        void setRenderType(SceneRenderType type);

        game::Scene* getScene(std::string name = "");
        SceneRenderType getRenderType();

        private:
        std::map<std::string, game::Scene*> m_scenes;
        std::string m_currentScene;
        SceneRenderType m_renderType;
    };
}

#endif