#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vars.hpp>
#include <resource.hpp>
#include <settings.hpp>
#include <filesystem>
#include <thread>
#include <vector>
#include "utils.hpp"

std::unordered_map<std::string, game::resource::Audio*> Musics;
std::unordered_map<std::string, game::resource::Audio*> Sounds;
std::unordered_map<std::string, sf::Texture*> Textures;
std::unordered_map<std::string, sf::Font*> Fonts;
std::unordered_map<std::string, sf::Shader*> Shaders;

static bool ALL_LOADED = false;
std::string RESSOURCE_ERROR;

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

        namespace shader{
            sf::Shader* loadFromFile(std::string mask, std::vector<std::pair<std::string,std::string>> pathsPair){
                sf::Shader* shader = new sf::Shader();

                for(auto [path, ext]: pathsPair){
                    std::string fullPath(path);
                    fullPath.append(ext);
                    if(!ext.compare(".vert")){
                        if(!shader->loadFromFile(path, sf::Shader::Type::Vertex)){
                            delete shader;
                            return nullptr;
                        }
                    }else if(!ext.compare(".frag")){
                        if(!shader->loadFromFile(path, sf::Shader::Type::Fragment)){
                            delete shader;
                            return nullptr;
                        }
                    }else if(!ext.compare(".geom") && game::vars::get<bool>("system.sfml.shader_geometry.active")){
                        if(!shader->loadFromFile(path, sf::Shader::Type::Geometry)){
                            delete shader;
                            return nullptr;
                        }
                    }
                }
                Shaders[mask] = shader;
                return shader;
            }
            void remove(std::string mask){
                if(!Shaders.contains(mask)) return;

                delete Shaders[mask];
                Shaders.erase(mask);
            }
            sf::Shader* get(std::string mask){
                if(!Shaders.contains(mask)) return nullptr;

                return Shaders[mask];
            }
            std::string mask(sf::Shader* shader){
                auto it = std::find_if(Shaders.begin(), Shaders.end(), [&shader](const auto& pair){
                    return shader == pair.second;
                });

                if(it != Shaders.end()){
                    return it->first;
                }

                return "";
            }
        }

        static void loadCallback(sf::RenderWindow *render){
            // TODO: Generate hash for resources to validate
            std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> groupedShaders;

            for(auto path: game::settings::getPropertyNode("config.paths.resources")){
                for(auto iter: std::filesystem::directory_iterator(path.as<std::string>())){
                    auto ext = iter.path().filename().extension();
                    std::string fileName = iter.path().filename().string();
                    void* loadedFile = nullptr;

                    int dotPosition = fileName.find(".");
                    fileName.erase(fileName.begin()+dotPosition, fileName.end());


                    if(!ext.compare(".mp3") || !ext.compare(".wav")){
                        auto type = iter.path().filename().generic_string().starts_with("bg") ? \
                                    game::resource::Audio::Type::MUSIC : game::resource::Audio::Type::SOUND;

                        loadedFile = audio::loadFromFile(fileName, iter.path().string(), type);
                    }else if(!ext.compare(".png") || !ext.compare(".jpg") || !ext.compare(".bmp")){
                        loadedFile = texture::loadFromFile(fileName, iter.path().string());
                    }else if(!ext.compare(".frag") || !ext.compare(".vert") || !ext.compare(".geom")){
                        std::vector<std::string> divisor;
                        game::string::split(fileName, divisor, ':');

                        groupedShaders[divisor[0]].push_back({iter.path().string(), ext.string()});
                        continue;;
                    }

                    if(render){
                        render->clear();

                        sf::Font font;
                        if(!font.openFromFile(game::settings::getProperty<std::string>("config.style.font"))){
                            render->close();
                            return;
                        }
                        sf::Text msg(font, "Loading: " + iter.path().generic_string(), 16);
                        msg.setPosition(sf::Vector2f(10, WINDOW_HEIGHT - 26));
                        if(!loadedFile){
                            msg.setString("Error to load file: " + iter.path().generic_string());
                            msg.setFillColor(sf::Color::Red);
                            render->draw(msg);
                            render->display();
                            sf::sleep(sf::seconds(4));
                            render->close();
                            return;
                        }else{
                            render->draw(msg);
                        }
                    }

                    if(!loadedFile){
                        RESSOURCE_ERROR = std::string("Error to load file: " + iter.path().generic_string());
                    }

                    if(render) render->display();
                }
            }
            for(auto [key, vector]: groupedShaders){
                game::resource::shader::loadFromFile(key, vector);
            }
        }

        std::thread loadAll(){
            if(!ALL_LOADED){
                ALL_LOADED = true;
                std::thread thread(loadCallback, nullptr);
                return thread;
            }

            return std::thread();
        }
        void loadAll(sf::RenderWindow &render){
            if(!ALL_LOADED) loadCallback(&render);
            ALL_LOADED = true;
        }
        std::string getError(){
            return RESSOURCE_ERROR;
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