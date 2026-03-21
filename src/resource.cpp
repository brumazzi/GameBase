#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <resource.hpp>
#include <settings.hpp>
#include <filesystem>

std::map<std::string, game::resource::Audio*> Musics;
std::map<std::string, game::resource::Audio*> Sounds;
std::map<std::string, sf::Texture*> Textures;

namespace game{
    namespace resource{
        namespace audio{
            Audio* loadFromFile(std::string mask, std::string path, Audio::Type type){
                Audio *audio = new Audio(path, type);

                if(!(audio->isMusic() || audio->isSound())){
                    delete audio;
                    return nullptr;
                }


                if(audio->isMusic()){
                    if(Musics.contains(mask)) delete Musics[mask];
                    Musics[mask] = audio;
                }else{
                    if(Sounds.contains(mask)) delete Sounds[mask];
                    Sounds[mask] = audio;
                }
                return audio;
            }
            // bool loadFromMemory(std::string mask,void *buff, std::size_t size, Audio::Type type){}
            void remove(std::string mask, Audio::Type type){
                Audio* audio = nullptr;
                if(type == Audio::Type::MUSIC){
                    if(!Musics.contains(mask)) return;

                    audio = Musics[mask];
                    Musics.erase(mask);
                }else if(type == Audio::Type::SOUND){
                    if(!Sounds.contains(mask)) return;

                    audio = Sounds[mask];
                    Sounds.erase(mask);
                }else return;

                delete audio;
            }
            Audio* get(std::string mask, Audio::Type type){
                Audio* audio = nullptr;
                if(type == Audio::Type::MUSIC){
                    if(!Musics.contains(mask)) return audio;
                    audio = Musics[mask];
                }else if(type == Audio::Type::SOUND){
                    if(!Sounds.contains(mask)) return audio;
                    audio = Sounds[mask];
                }

                return audio;
            }
            std::string mask(Audio* audio, Audio::Type type){
                if(type == Audio::Type::MUSIC){
                    auto it = std::find_if(Musics.begin(), Musics.end(), [&audio](const auto& pair){
                        return audio == pair.second;
                    });

                    if(it != Musics.end()){
                        return it->first;
                    }
                }else if(type == Audio::Type::SOUND){
                    auto it = std::find_if(Sounds.begin(), Sounds.end(), [&audio](const auto& pair){
                        return audio == pair.second;
                    });

                    if(it != Sounds.end()){
                        return it->first;
                    }
                }
                return "";
            }
        }

        namespace texture{
            sf::Texture* loadFromFile(std::string mask, std::string path){
                sf::Texture *texture = new sf::Texture();
                if(!texture->loadFromFile(path)){
                    delete texture;
                    return nullptr;
                }

                if(Textures.contains(mask)){
                    delete Textures[mask];
                }

                Textures[mask] = texture;

                return texture;

            }
            // sf::Texture* loadFromMemory(std::string mask,void *buff, std::size_t size){}
            void remove(std::string mask){
                if(!Textures.contains(mask)) return;

                delete Textures[mask];
                Textures.erase(mask);
            }
            sf::Texture* get(std::string mask){
                if(!Textures.contains(mask)) return nullptr;

                return Textures[mask];
            }
            std::string mask(sf::Texture* texture){
                auto it = std::find_if(Textures.begin(), Textures.end(), [&texture](const auto& pair){
                    return texture == pair.second;
                });

                if(it != Textures.end()){
                    return it->first;
                }

                return "";
            }
        }

        static void loadCallback(sf::RenderWindow *render){
            for(auto path: game::settings::getPropertyNode("config.paths.resources")){
                for(auto iter: std::filesystem::directory_iterator(path.as<std::string>())){
                    auto ext = iter.path().filename().extension();
                    if(!ext.compare(".mp3") || !ext.compare(".wav")){
                        auto type = iter.path().filename().generic_string().starts_with("bg") ? \
                                    game::resource::Audio::Type::MUSIC : game::resource::Audio::Type::SOUND;

                        auto fileName = iter.path().filename().string();
                        fileName.erase(fileName.end()-4, fileName.end());
                        Audio* loadedFile = audio::loadFromFile(fileName, iter.path().string(), type);

                        if(render){
                            render->clear();

                            sf::Font font;
                            if(!font.openFromFile(game::settings::getProperty<std::string>("config.style.font"))){
                                render->close();
                                return;
                            }
                            sf::Text msg(font, "Loading: " + iter.path().generic_string(), 16);
                            msg.setPosition(sf::Vector2f(10, render->getSize().y - 26));
                            if(!loadedFile){
                                msg.setString("Error to load file: " + iter.path().generic_string());
                                msg.setFillColor(sf::Color::Red);
                                render->draw(msg);
                                render->display();
                                sf::sleep(sf::seconds(2));
                                render->close();
                                return;
                            }else{
                                render->draw(msg);
                            }
                        }
                    }else if(!ext.compare(".png") || !ext.compare(".jpg") || !ext.compare(".bmp")){
                        auto fileName = iter.path().filename().string();
                        fileName.erase(fileName.end()-4, fileName.end());
                        sf::Texture* loadedFile = texture::loadFromFile(fileName, iter.path().string());

                        if(render){
                            render->clear();

                            sf::Font font;
                            if(!font.openFromFile(game::settings::getProperty<std::string>("config.style.font"))){
                                render->close();
                                return;
                            }
                            sf::Text msg(font, "Loading: " + iter.path().generic_string(), 16);
                            msg.setPosition(sf::Vector2f(10, render->getSize().y - 26));
                            if(!loadedFile){
                                msg.setString("Error to load file: " + iter.path().generic_string());
                                msg.setFillColor(sf::Color::Red);
                                render->draw(msg);
                                render->display();
                                sf::sleep(sf::seconds(2));
                                render->close();
                                return;
                            }else{
                                render->draw(msg);
                            }
                        }
                    }

                    if(render) render->display();
                }
            }
        }

        std::thread loadAll(){
            std::thread thread(loadCallback, nullptr);
            return thread;
        }
        void loadAll(sf::RenderWindow &render){
            loadCallback(&render);
        }
        void unloadAll(){
            for(auto it: Musics){
                delete it.second;
            }
            for(auto it: Sounds){
                delete it.second;
            }
            for(auto it: Textures){
                delete it.second;
            }
            Musics.clear();
            Sounds.clear();
            Textures.clear();
        }
    }
}