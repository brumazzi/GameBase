#include <resource.hpp>
#include <filesystem>

namespace game{
    namespace resource{
        Audio::Audio(std::string path, Audio::Type type){
            this->m_music = nullptr;
            this->m_sound = nullptr;
            this->m_soundBuffer = nullptr;

            this->m_type = type;
            if(this->isMusic()){
                this->m_music = new sf::Music();
                if(!this->m_music->openFromFile(path)) this->m_type = Audio::Type::NONE;
            }else if(this->isSound()){
                this->m_soundBuffer = new sf::SoundBuffer();
                if(this->m_soundBuffer->loadFromFile(path))
                    this->m_sound = new sf::Sound(*this->m_soundBuffer);
                else
                    this->m_type = Audio::Type::NONE;
            }
        }
        Audio::~Audio(){
            if(this->m_music) delete this->m_music;
            if(this->m_sound) delete this->m_sound;
            if(this->m_soundBuffer) delete this->m_soundBuffer;
        }

        bool Audio::isSound(){
            return this->m_type == Audio::Type::SOUND;
        }
        bool Audio::isMusic(){
            return this->m_type == Audio::Type::MUSIC;
        }
        bool Audio::isLoop(){
            if(this->m_type == Audio::Type::NONE) return false;

            return (this->isSound()) ? this->m_sound->isLooping() : this->m_music->isLooping();
        }
        bool Audio::isPlaying(){
            if(this->m_type == Audio::Type::NONE) return false;

            return (this->isSound()) ?
                    this->m_sound->getStatus() == sf::Sound::Status::Playing :
                    this->m_music->getStatus() == sf::Music::Status::Playing;
        }
        bool Audio::isPaused(){
            if(this->m_type == Audio::Type::NONE) return false;

            return (this->isSound()) ?
                    this->m_sound->getStatus() == sf::Sound::Status::Paused :
                    this->m_music->getStatus() == sf::Music::Status::Paused;
        }
        bool Audio::isStoped(){
            if(this->m_type == Audio::Type::NONE) return false;

            return (this->isSound()) ?
                    this->m_sound->getStatus() == sf::Sound::Status::Stopped :
                    this->m_music->getStatus() == sf::Music::Status::Stopped;
        }

        void Audio::play(){
            if(this->isMusic()){
                this->m_music->play();
            }else if(this->isSound()){
                this->m_sound->play();
            }
        }
        void Audio::pause(){
            if(this->isMusic()){
                this->m_music->pause();
            }else if(this->isSound()){
                this->m_sound->pause();
            }
        }
        void Audio::stop(){
            if(this->isMusic()){
                this->m_music->stop();
            }else if(this->isSound()){
                this->m_sound->stop();
            }
        }

        void Audio::setLoop(bool loop){
            if(this->isMusic()){
                this->m_music->setLooping(loop);
            }else if(this->isSound()){
                this->m_sound->setLooping(loop);
            }
        }
        void Audio::setVolume(float volume){
            if(this->isMusic()){
                this->m_music->setVolume(volume);
            }else if(this->isSound()){
                this->m_sound->setVolume(volume);
            }
        }

        float Audio::getVolume(){
            if(this->isMusic()){
                return this->m_music->getVolume();
            }else if(this->isSound()){
                return this->m_sound->getVolume();
            }

            return 0.0;
        }
    }
}