#ifndef __RESOURCE_HPP__
#define __RESOURCE_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <iostream>
#include <thread>
#include <map>

namespace game{
    namespace resource{
        class Audio{
            public:
            enum class Type { NONE, SOUND, MUSIC};

            Audio(std::string path, Type type);
            ~Audio();
            bool isSound();
            bool isMusic();
            bool isLoop();
            bool isPlaying();
            bool isPaused();
            bool isStoped();

            void play();
            void pause();
            void stop();

            void setLoop(bool loop);
            void setVolume(float volume);

            float getVolume();

            private:
            sf::Sound *m_sound;
            sf::SoundBuffer *m_soundBuffer;
            sf::Music *m_music;
            Type m_type;
        };

        namespace audio{
            Audio* loadFromFile(std::string mask, std::string path, Audio::Type type);
            // Audio* loadFromMemory(std::string mask,void *buff, std::size_t size, Audio::Type type);
            void remove(std::string mask, Audio::Type type);
            Audio* get(std::string mask, Audio::Type type);
            std::string mask(Audio* audio, Audio::Type type);
        }

        namespace texture{
            sf::Texture* loadFromFile(std::string mask, std::string path);
            // sf::Texture* loadFromMemory(std::string mask,void *buff, std::size_t size);
            void remove(std::string mask);
            sf::Texture* get(std::string mask);
            std::string mask(sf::Texture* texture);
        }

        namespace shader{
            sf::Shader* loadFromFile(std::string mask, std::vector<std::pair<std::string,std::string>> pathsPair);
            void remove(std::string mask);
            sf::Shader* get(std::string mask);
            std::string mask(sf::Shader* shader);
        }

        std::thread loadAll();
        void loadAll(sf::RenderWindow &render);
        std::string getError();
        void unloadAll();
    }
}

#endif