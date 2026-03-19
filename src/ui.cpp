#include <tinyxml2.h>
#include <imgui/imgui.h>

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <ui.hpp>
#include <vector>
#include <utils.hpp>
#include <algorithm>
#include <settings.hpp>
#include "imgui-SFML.h"

using namespace ImGui;

// void renderRecursive(game::ui::XMLWindow* root, tinyxml2::XMLElement* element);

namespace game {
    namespace ui {
        ImFont* loadConfigFont(ImGuiIO& io){
            io.Fonts->ClearFonts();
            ImFont* font = io.Fonts->AddFontFromFileTTF(game::settings::getProperty<std::string>("config.style.font").c_str(), 12, nullptr, io.Fonts->GetGlyphRangesDefault());
            // ImFont* font = io.Fonts->AddFontFromFileTTF("/home/brumazzi/.fonts/altcoins-v1.ttf", 12, nullptr, io.Fonts->GetGlyphRangesDefault());
            if(!font){
                io.Fonts->AddFontDefault();
                return nullptr;
            }
            io.Fonts->Build();
            SFML::UpdateFontTexture();

            io.FontDefault = font;
            unsigned char* pixels;
            int width, height;
            io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

            return font;
        }
    }  // namespace ui
}  // namespace game
