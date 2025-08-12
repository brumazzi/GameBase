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
            sfg::XMLWidget XMLWidget;
            if(!XMLWidget.loadFromFile("./resources/gui/SettingsWindow.xml")){
                std::cerr << "\"SettingsWindow.xml\" not fount!"  << std::endl;
                return 1;
            }

            sfg::Widget::Ptr widget = XMLWidget.getWidget();
            if(!widget){
                std::cerr << "Fail to start settings window!"  << std::endl;
                return 2;
            }

            std::dynamic_pointer_cast<sfg::Window>(widget)->GetSignal(sfg::Window::OnCloseButton).Connect([widget, &window, &endGame] {
                endGame = true;
                window.close();
            });

            game::translate::applyToWidget(widget);

            sfg::Button::Ptr buttonPlay = std::dynamic_pointer_cast<sfg::Button>(widget->GetWidgetById("button-play"));
            sfg::Button::Ptr buttonQuit = std::dynamic_pointer_cast<sfg::Button>(widget->GetWidgetById("button-quit"));
            sfg::CheckButton::Ptr inputFullscreen = std::dynamic_pointer_cast<sfg::CheckButton>(widget->GetWidgetById("input-fullscreen"));
            sfg::ComboBox::Ptr inputResolution = std::dynamic_pointer_cast<sfg::ComboBox>(widget->GetWidgetById("input-resolution"));

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

            inputFullscreen->SetActive(game::settings::getProperty<bool>("config.style.window.fullscreen"));

            uint buttonPlaySignal = buttonPlay->GetSignal(sfg::Button::OnLeftClick).Connect([&window]{window.close();});
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
            buttonPlay->GetSignal(sfg::Button::OnLeftClick).Disconnect(buttonPlaySignal);
            buttonQuit->GetSignal(sfg::Button::OnLeftClick).Disconnect(buttonQuitSignal);

            int width=0,height=0;
            sscanf(inputResolution->GetSelectedText().toAnsiString().c_str(), "%dx%d", &width, &height);

            game::settings::setProperty("config.style.window.fullscreen", inputFullscreen->IsActive());
            game::settings::setProperty<int>("config.style.window.width", width);
            game::settings::setProperty<int>("config.style.window.height", height);

            return endGame;
        }
    }
}