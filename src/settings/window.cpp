#include <settings.hpp>
#include <filesystem>
#include <buttons.hpp>
#include <XMLLoader/XMLLoader.hpp>

#include <SFGUI/SFGUI.hpp>
namespace game{
    namespace settings{
        bool openSettingWindow(sf::RenderWindow &window){
            bool endGame = false;
            sfg::SFGUI sfgUI;
            sfg::ui::XMLLoader::Ptr XMLLoader = sfg::ui::XMLLoader::Create();
            if(!XMLLoader->loadFromFile("./resources/gui/SettingsWindow.xml", &window)){
                std::cerr << "\"SettingsWindow.xml\" not fount!"  << std::endl;
                return 1;
            }

            sfg::Widget::Ptr widget = XMLLoader->getWidget();
            if(!widget){
                std::cerr << "Fail to start settings window!"  << std::endl;
                return 2;
            }

            std::dynamic_pointer_cast<sfg::Window>(widget)->GetSignal(sfg::Window::OnCloseButton).Connect([widget, &window, &endGame] {
                endGame = true;
                window.close();
            });

            sfg::Button::Ptr buttonQuit = std::dynamic_pointer_cast<sfg::Button>(widget->GetWidgetById("button-quit"));

            uint buttonQuitSignal = buttonQuit->GetSignal(sfg::Button::OnLeftClick).Connect([&window, &endGame]{
                endGame = true;
                window.close();
            });

            sfg::Desktop desktopUI;
            desktopUI.LoadThemeFromFile("./resources/gui/themes/default.css");
            desktopUI.Add(widget);

            while(window.isOpen()){
                while(std::optional event = window.pollEvent()){
                    desktopUI.HandleEvent(*event);
                    if(event->is<sf::Event::Closed>()){
                        window.close();
                        endGame = true;
                    }

                }

                sf::Vector2f widgetPos = widget->GetAbsolutePosition();
                if(widgetPos.x != -1 || widgetPos.y != -1){
                    sf::Vector2i newPosition = window.getPosition();
                    newPosition.x += widgetPos.x;
                    newPosition.y += widgetPos.y;

                    window.setPosition(newPosition);
                    widget->SetPosition({-1, -1});
                }

                window.clear(sf::Color::White);
                desktopUI.Update(0.0f);

                sfgUI.Display(window);
                window.display();
            }
            std::dynamic_pointer_cast<sfg::Window>(widget)->GetSignal(sfg::Window::OnCloseButton).Disconnect(
                std::dynamic_pointer_cast<sfg::Window>(widget)->GetSignal(sfg::Window::OnCloseButton).GetGUID()
            );
            buttonQuit->GetSignal(sfg::Button::OnLeftClick).Disconnect(buttonQuitSignal);

            return endGame;
        }
    }
}

// window events

extern "C" void config_windowLoad(sfg::Window::Ptr window, sf::RenderWindow* render){
    game::translate::applyToWidget(std::dynamic_pointer_cast<sfg::Widget>(window));

    sfg::CheckButton::Ptr inputFullscreen = std::dynamic_pointer_cast<sfg::CheckButton>(window->GetWidgetById("input-fullscreen"));
    sfg::CheckButton::Ptr inputVSync = std::dynamic_pointer_cast<sfg::CheckButton>(window->GetWidgetById("input-vsync"));

    inputFullscreen->SetActive(game::settings::getProperty<bool>("config.style.window.fullscreen"));
    inputVSync->SetActive(game::settings::getProperty<bool>("config.game.vsync"));

    sfg::ComboBox::Ptr inputResolution = std::dynamic_pointer_cast<sfg::ComboBox>(window->GetWidgetById("input-resolution"));
    // std::vector<sf::VideoMode> modes(sf::VideoMode::getFullscreenModes());

    // for(const auto mode: modes){
    //     std::cout << std::string(mode.size.x+"x"+mode.size.y) << std::endl;
    //     inputResolution->AppendItem(std::string(mode.size.x+"x"+mode.size.y));
    // }

    for(int index = 0; index < inputResolution->GetItemCount(); index++){
        bool res = !inputResolution->GetItem(index).toUtf32().compare(
            sf::String(
                std::string(game::settings::getProperty<std::string>("config.style.window.width")) + "x" + std::string(game::settings::getProperty<std::string>("config.style.window.height"))
            ).toUtf32()
        );
        if(res){
            inputResolution->SelectItem(index);
            break;
        }
    }

}
extern "C" void config_gamePlay(sfg::Window::Ptr window, sf::RenderWindow* render){
    render->close();
}
extern "C" void config_setResolution(sfg::ComboBox::Ptr combo, sf::RenderWindow* render){
    int width=0,height=0;
    sscanf(combo->GetSelectedText().toAnsiString().c_str(), "%dx%d", &width, &height);

    game::settings::setProperty<int>("config.style.window.width", width);
    game::settings::setProperty<int>("config.style.window.height", height);
}

extern "C" void config_setFullscreen(sfg::CheckButton::Ptr check, sf::RenderWindow* render){
    game::settings::setProperty("config.style.window.fullscreen", check->IsActive());
}
extern "C" void config_setVSync(sfg::CheckButton::Ptr check, sf::RenderWindow* render){
    game::settings::setProperty("config.game.vsync", check->IsActive());
}
