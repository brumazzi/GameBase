#include <ui.hpp>
#include <imgui-SFML.h>
#include "imgui/imgui.h"

namespace game{
    namespace ui{
        int settingsWindow(sf::RenderWindow& render){
            ImGui::SFML::Init(render);

            sf::Clock deltaClock;
            int status = 0;
            while (render.isOpen()) {
                while (auto event = render.pollEvent()) {
                    ImGui::SFML::ProcessEvent(render, *event);

                    if (event->is<sf::Event::Closed>()) {
                        render.close();
                    }
                }

                ImGuiIO& io = ImGui::GetIO();
                if (io.WantCaptureMouse){
                    // ImGui está usando o mouse (ex.: hover em janela) - ignore na sua app
                    // (Sua lógica de mouse da app aqui, se não capturado)
                }

                ImGui::SFML::Update(render, deltaClock.restart());

                render.clear(sf::Color(0xccccccff));

                bool btnQuit = false;
                bool btnPlay = false;

                auto size = render.getSize();
                if(Begin("Settings/Container", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)){
                    SetWindowSize({(float)size.x-2.0f, (float)size.y*0.9f-1.f});
                    SetWindowPos({1, 1});
                    if(BeginTabBar("Settings/Container/TabBar")){
                        if(BeginTabItem("General")){
                            Button("Save General");
                            EndTabItem();
                        }
                        if(BeginTabItem("Video")){
                            Button("Save Video");
                            EndTabItem();
                        }
                        if(BeginTabItem("Audio")){
                            Text("Volume");
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

                    btnQuit = Button("Quit", {GetWindowWidth()-16, GetWindowHeight()-16});
                    End();
                }

                if(Begin("Settings/Button/Play", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)){
                    SetWindowSize({(float)size.x*0.5f-2.0f, (float)size.y*0.1f-2.0f});
                    SetWindowPos({(float)size.x*0.5f+1, size.y*0.9f+1.0f});
                    btnPlay = Button("Play", {GetWindowWidth()-16, GetWindowHeight()-16});
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

            // Finaliza ImGui-SFML
            ImGui::SFML::Shutdown();
            return status;
        }
    }
}