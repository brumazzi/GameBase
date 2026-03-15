#include <imgui-SFML.h>
#include <imgui/imgui.h>

#include <SFML/Graphics.hpp>
#include <tinyxml2.h>
#include <SFML/Graphics/RenderWindow.hpp>

using namespace ImGui;

namespace game {
    namespace ui {
        // enum error{
        //     NO_ERROR,
        //     UI_NOT_ACTIVATED,
        //     CANCEL_START
        // };

        // inline  static const std::map<std::string, int> WINDOW_FLAGS = {
        //     {"none", (int) ImGuiWindowFlags_None},
        //     {"notitlebar", (int) ImGuiWindowFlags_NoTitleBar},
        //     {"noresize", (int) ImGuiWindowFlags_NoResize},
        //     {"nomove", (int) ImGuiWindowFlags_NoMove},
        //     {"noscrollbar", (int) ImGuiWindowFlags_NoScrollbar},
        //     {"noscrollwithmouse", (int) ImGuiWindowFlags_NoScrollWithMouse},
        //     {"nocollapse", (int) ImGuiWindowFlags_NoCollapse},
        //     {"alwaysautoresize", (int) ImGuiWindowFlags_AlwaysAutoResize},
        //     {"nobackground", (int) ImGuiWindowFlags_NoBackground},
        //     {"nosavedsettings", (int) ImGuiWindowFlags_NoSavedSettings},
        //     {"nomouseinputs", (int) ImGuiWindowFlags_NoMouseInputs},
        //     {"menubar", (int) ImGuiWindowFlags_MenuBar},
        //     {"horizontalscrollbar", (int) ImGuiWindowFlags_HorizontalScrollbar},
        //     {"nofocusonappearing", (int) ImGuiWindowFlags_NoFocusOnAppearing},
        //     {"nobringtofrontonfocus", (int) ImGuiWindowFlags_NoBringToFrontOnFocus},
        //     {"alwaysverticalscrollbar", (int) ImGuiWindowFlags_AlwaysVerticalScrollbar},
        //     {"alwayshorizontalscrollbar", (int) ImGuiWindowFlags_AlwaysHorizontalScrollbar},
        //     {"nonavinputs", (int) ImGuiWindowFlags_NoNavInputs},
        //     {"nonavfocus", (int) ImGuiWindowFlags_NoNavFocus},
        //     {"unsaveddocument", (int) ImGuiWindowFlags_UnsavedDocument},
        //     {"nonav", (int) ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus},
        //     {"nodecoration", (int) ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse},
        //     {"noinputs", (int) ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus},
        //     {"childwindow", (int) ImGuiWindowFlags_ChildWindow},
        //     {"tooltip", (int) ImGuiWindowFlags_Tooltip},
        //     {"popup", (int) ImGuiWindowFlags_Popup},
        //     {"modal", (int) ImGuiWindowFlags_Modal},
        //     {"childmenu", (int) ImGuiWindowFlags_ChildMenu}
        // };

        // class Window{
        //     public:
        //         Window(tinyxml2::XMLDocument& document);
        //         void render();
        //     private:
        //         void buildRecursive(tinyxml2::XMLElement* element);
        //         std::unordered_map<std::string, std::map<std::string, std::string>> m_components;
        //         std::unordered_map<std::string, char [256]> m_inputs;
        // };

        // bool loadWindow(std::string name);
        // void getWindows(std::vector<std::string>& list);
        // int showWindow(std::string name, sf::RenderWindow& window);
        // void freeWindow(std::string name);
        // void clearWindows();

        int settingsWindow(sf::RenderWindow& render);
    }  // namespace ui
}  // namespace game