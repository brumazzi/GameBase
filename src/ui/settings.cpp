#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>
#include <string>
#include <ui.hpp>
#include <imgui-SFML.h>
#include "imgui/imgui.h"
#include "utils.hpp"
#include <iostream>
#include <settings.hpp>

namespace game{
    namespace ui{
        int settingsWindow(sf::RenderWindow& render){
            int status = ImGui::SFML::Init(render);
            if(!status){
                // while (render.isOpen()) {

                // }
                return -1;
            }

            sf::Clock deltaClock;
            status = 1;

            bool fullscreen = game::settings::getProperty<bool>("config.style.window.fullscreen");
            bool vsync = game::settings::getProperty<bool>("config.game.vsync");
            int currentResolution = 0;
            const char* resolutions[] = {
                "1024x576",
                "1280x720",
                "1366x768",
                "1368x768",
                "1600x900",
                "1920x1080",
                "2590x1440"
            };
            int currentLang = 0;
            const char* langs[] = {
                "en-US",
                "pt-BR"
            };
            std::string configResolution(
                std::to_string(game::settings::getProperty<int>("config.style.window.width"))
                + "x" +
                std::to_string(game::settings::getProperty<int>("config.style.window.height"))
            );
            for(int x=0; x<7; x++){
                if(!std::string(resolutions[x]).compare(configResolution)){
                    currentResolution = x;
                    break;
                }
            }
            for(int x=0; x<2; x++){
                if(!std::string(langs[x]).compare(game::settings::getProperty<std::string>("config.lang"))){
                    currentLang = x;
                    break;
                }
            }

            ImGuiIO& io = ImGui::GetIO();
            auto font = loadConfigFont(io);
            if(font) PushFont(font);

            while (render.isOpen()) {
                while (auto event = render.pollEvent()) {
                    ImGui::SFML::ProcessEvent(render, *event);

                    if (event->is<sf::Event::Closed>()) {
                        render.close();
                    }
                }

                if (io.WantCaptureMouse){
                    // ImGui está usando o mouse (ex.: hover em janela) - ignore na sua app
                    // (Sua lógica de mouse da app aqui, se não capturado)
                }

                ImGui::SFML::Update(render, deltaClock.restart());

                render.clear(sf::Color(0xccccccff));

                bool btnQuit = false;
                bool btnPlay = false;

                auto size = render.getSize();
                if(Begin("##Settings", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)){
                    SetWindowSize({(float)size.x-2.0f, (float)size.y*0.9f-1.f});
                    SetWindowPos({1, 1});
                    if(BeginTabBar("##Settings/TabBar")){
                        if(BeginTabItem(std::string(t("setting.window.tab.general")+"##Settings/General").c_str())){
                            Text("%s", t("setting.window.label.language").c_str());
                            if(Combo("##Settings/Language", &currentLang, langs, 2)){
                                // Here the code update language and window title in real-time
                                game::settings::setProperty<std::string>("config.lang", std::string(langs[currentLang]));
                                render.setTitle(game::string::str_to_utf32(t("setting.window.title")));
                            }
                            EndTabItem();
                        }
                        if(BeginTabItem(std::string(t("setting.window.tab.video")+"##Settings/Video").c_str())){
                            Text("%s", std::string(t("setting.window.label.resolution")).c_str());
                            Combo("##Settings/Resolution", &currentResolution, resolutions, 7);
                            Checkbox(std::string(t("setting.window.checkbox.fullscreen")+"##Settings/Fullscreen").c_str(), &fullscreen);SameLine();
                            Checkbox(std::string(t("setting.window.checkbox.vsync")+"##Settings/V-Sync").c_str(), &vsync);
                            EndTabItem();
                        }
                        if(BeginTabItem(std::string(t("setting.window.tab.audio")+"##Settings/Audio").c_str())){
                            TextColored({0.9, 0.9, 0.0, 1.0}, "In development");
                            EndTabItem();
                        }
                        EndTabBar();
                    }
                    End();
                }
                PushStyleVar(ImGuiStyleVar_WindowPadding, {8.0f, 8.0f});
                if(Begin("Settings/Button/Quit", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)){
                    SetWindowSize({(float)size.x*0.5f-1, (float)size.y*0.1f-2.0f});
                    SetWindowPos({1, size.y*0.9f+1.0f});

                    ImGui::PushStyleColor(ImGuiCol_Button,         (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.4f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.5f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,   (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.6f));
                    btnQuit = Button(std::string(t("setting.window.button.quit")+"##Settings/Quit").c_str(), {GetWindowWidth()-16, GetWindowHeight()-16});
                    ImGui::PopStyleColor(3);
                    End();
                }

                if(Begin("Settings/Button/Play", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)){
                    SetWindowSize({(float)size.x*0.5f-2.0f, (float)size.y*0.1f-2.0f});
                    SetWindowPos({(float)size.x*0.5f+1, size.y*0.9f+1.0f});
                    ImGui::PushStyleColor(ImGuiCol_Button,         (ImVec4)ImColor::HSV(3 / 7.0f, 0.6f, 0.4f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,  (ImVec4)ImColor::HSV(3 / 7.0f, 0.7f, 0.5f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,   (ImVec4)ImColor::HSV(3 / 7.0f, 0.8f, 0.6f));
                    btnPlay = Button(std::string(t("setting.window.button.play")+"##Settings/Play").c_str(), {GetWindowWidth()-16, GetWindowHeight()-16});
                    ImGui::PopStyleColor(3);
                    End();
                }
                PopStyleVar();

                ImGui::SFML::Render(render);

                render.display();

                if(btnQuit){
                    status = 1;
                    render.close();
                }else if(btnPlay){
                    status = 0;
                    render.close();
                }
            }

            if(!status){
                game::settings::setProperty<bool>("config.style.window.fullscreen", fullscreen);
                game::settings::setProperty<bool>("config.game.vsync", vsync);
                int w,h;
                sscanf(resolutions[currentResolution], "%ux%u", &w, &h);
                game::settings::setProperty<int>("config.style.window.width", w);
                game::settings::setProperty<int>("config.style.window.height", h);
            }

            // Finaliza ImGui-SFML
            if(font) PopFont();
            ImGui::SFML::Shutdown();
            return status;
        }
    }
}