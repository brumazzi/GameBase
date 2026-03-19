#include <settings.hpp>
#include <filesystem>
#include <utils.hpp>

static YAML::Node Translates;

#define if_translate(label) if(label.size() > 2 && label[0] == 't' && label[1] == ':')

namespace game{
    namespace translate{

        static YAML::Node translatesFindRecursive(const YAML::Node &node, const std::vector<std::string> &path, const uint64_t index){
            for(auto it: node){
                if(!it.first.as<std::string>().compare(path[index])){
                    if(index == path.size()-1){
                        return it.second;
                    }
                    else return translatesFindRecursive(it.second, path, index+1);
                }
            }

            YAML::Node nullNode;
            return nullNode;
        }

        void load(){
            for(auto it: std::filesystem::directory_iterator(::game::settings::getProperty<std::string>("config.paths.translates"))){
                if(it.path().extension().string() == ".yaml" || it.path().extension().string() == ".yml"){
                    YAML::Node node = YAML::LoadFile(it.path().string());
                    std::string name = it.path().filename().string();

                    uint8_t extSize = (it.path().extension().string() == ".yaml") ? 5 : 4;
                    std::string lang = name.replace(name.end()-extSize, name.end(), "");

                    Translates[lang] = node[lang];
                }
            }
        }
        std::string get(std::string path){
            std::vector<std::string> splitedPath;
            game::string::split(path, splitedPath, '.');
            YAML::Node node = translatesFindRecursive(Translates[::game::settings::getProperty<std::string>("config.lang")], splitedPath, 0);
            if(node.IsScalar()) return node.as<std::string>();

            return path;
        }
        void setLang(std::string lang){
            ::game::settings::getSettings()["config"]["lang"] = lang;
        }

        // void applyToWidget(sfg::Widget::Ptr widget){
        //     std::string widgetType = widget->GetName();

        //     if(!widgetType.compare("Box")){
        //         auto w = std::static_pointer_cast<sfg::Box>(widget);

        //         for(auto child: w->GetChildren()){
        //             applyToWidget(child);
        //         }
        //     }else if(!widgetType.compare("Button")){
        //         auto w = std::static_pointer_cast<sfg::Button>(widget);

        //         std::string label(w->GetLabel());
        //         if_translate(label){
        //             label.erase(0, 2);
        //             w->SetLabel(game::string::str_to_utf32(t(label)));
        //         }
        //     }else if(!widgetType.compare("CheckButton")){
        //         auto w = std::static_pointer_cast<sfg::CheckButton>(widget);

        //         std::string label(w->GetLabel());
        //         if_translate(label){
        //             label.erase(0, 2);
        //             w->SetLabel(game::string::str_to_utf32(t(label)));
        //         }
        //     }else if(!widgetType.compare("ComboBox")){
        //         auto w = std::static_pointer_cast<sfg::ComboBox>(widget);

        //         std::vector<std::string> translatedItems;
        //         uint selectedItem = w->GetSelectedItem();

        //         while(w->GetItemCount()){
        //             std::string item = w->GetItem(0);

        //             if_translate(item){
        //                 item.erase(0,2);
        //                 item = t(item);
        //             }
        //             translatedItems.push_back(item);
        //             w->RemoveItem(0);
        //         }

        //         for(auto item: translatedItems) w->AppendItem(game::string::str_to_utf32(item));
        //     }else if(!widgetType.compare("Fixed")){
        //         auto w = std::static_pointer_cast<sfg::Fixed>(widget);

        //         for(auto child: w->GetChildren()){
        //             applyToWidget(child);
        //         }
        //     }else if(!widgetType.compare("Frame")){
        //         auto w = std::static_pointer_cast<sfg::Frame>(widget);

        //         std::string label(w->GetLabel());

        //         for(auto child: w->GetChildren()){
        //             applyToWidget(child);
        //         }
        //         if_translate(label){
        //             label.erase(0, 2);
        //             w->SetLabel(game::string::str_to_utf32(t(label)));
        //         }
        //     }else if(!widgetType.compare("Label")){
        //         auto w = std::static_pointer_cast<sfg::Label>(widget);

        //         std::string label(w->GetText());
        //         if_translate(label){
        //             label.erase(0, 2);
        //             w->SetText(game::string::str_to_utf32(t(label)));
        //         }
        //     }else if(!widgetType.compare("Notebook")){
        //         auto w = std::static_pointer_cast<sfg::Notebook>(widget);

        //         for(int i=0; i<w->GetPageCount(); i++){
        //             applyToWidget(w->GetNthPage(i));

        //             auto page = std::static_pointer_cast<sfg::Label>(w->GetNthTabLabel(i));
        //             std::string label(page->GetText());
        //             if_translate(label){
        //                 label.erase(0, 2);
        //                 page->SetText(game::string::str_to_utf32(t(label)));
        //             }
        //         }
        //     }else if(!widgetType.compare("ProgressBar")){
        //         auto w = std::static_pointer_cast<sfg::ProgressBar>(widget);
        //     }else if(!widgetType.compare("RadioButton")){
        //         auto w = std::static_pointer_cast<sfg::RadioButton>(widget);

        //         std::string label(w->GetLabel());
        //         if_translate(label){
        //             label.erase(0, 2);
        //             w->SetLabel(game::string::str_to_utf32(t(label)));
        //         }
        //     }else if(!widgetType.compare("ScrolledWindow")){
        //         auto w = std::static_pointer_cast<sfg::ScrolledWindow>(widget);

        //         for(auto child: w->GetChildren()){
        //             applyToWidget(child);
        //         }
        //     }else if(!widgetType.compare("Table")){
        //         auto w = std::static_pointer_cast<sfg::Table>(widget);

        //         for(auto child: w->GetChildren()){
        //             applyToWidget(child);
        //         }
        //     }else if(!widgetType.compare("ToggleButton")){
        //         auto w = std::static_pointer_cast<sfg::ToggleButton>(widget);

        //         std::string label(w->GetLabel());
        //         if_translate(label){
        //             label.erase(0, 2);
        //             w->SetLabel(game::string::str_to_utf32(t(label)));
        //         }
        //     }else if(!widgetType.compare("Viewport")){
        //         auto w = std::static_pointer_cast<sfg::Viewport>(widget);

        //         for(auto child: w->GetChildren()){
        //             applyToWidget(child);
        //         }
        //     }else if(!widgetType.compare("Window")){
        //         auto w = std::static_pointer_cast<sfg::Window>(widget);

        //         std::string label(w->GetTitle());
        //         if_translate(label){
        //             label.erase(0, 2);
        //             w->SetTitle(game::string::str_to_utf32(t(label)));
        //         }
        //         for(auto child: w->GetChildren()){
        //             applyToWidget(child);
        //         }
        //     }
        // }
    }
}