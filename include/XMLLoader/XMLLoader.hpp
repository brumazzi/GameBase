#pragma once

#include <tinyxml2.h>

#include <SFGUI/Widgets.hpp>
#include <string>

#define sfg_cast(className, ptr) std::dynamic_pointer_cast<sfg::className>(ptr)

namespace sfg {
    namespace ui {
        const std::string OnStateChange        ("on-state-change");           // Widget event selector
        const std::string OnGainFocus          ("on-gain-focus");             // Widget event selector
        const std::string OnLostFocus          ("on-lost-focus");             // Widget event selector
        const std::string OnExpose             ("on-expose");                 // Widget event selector
        const std::string OnSizeAllocate       ("on-size-allocate");          // Widget event selector
        const std::string OnSizeRequest        ("on-size-request");           // Widget event selector
        const std::string OnMouseEnter         ("on-mouse-enter");            // Widget event selector
        const std::string OnMouseLeave         ("on-mouse-leave");            // Widget event selector
        const std::string OnMouseMove          ("on-mouse-move");             // Widget event selector
        const std::string OnMouseLeftPress     ("on-mouse-left-press");       // Widget event selector
        const std::string OnMouseRightPress    ("on-mouse-right-press");      // Widget event selector
        const std::string OnMouseLeftRelease   ("on-mouse-left-release");     // Widget event selector
        const std::string OnMouseRightRelease  ("on-mouse-right-release");    // Widget event selector
        const std::string OnLeftClick          ("on-left-click");             // Widget event selector
        const std::string OnRightClick         ("on-right-click");            // Widget event selector
        const std::string OnKeyPress           ("on-key-press");              // Widget event selector
        const std::string OnKeyRelease         ("on-key-release");            // Widget event selector
        const std::string OnText               ("on-text");                   // Widget event selector
        const std::string OnSelect             ("on-select");                 // ComboBox event selector
        const std::string OnOpen               ("on-open");                   // ComboBox event selector
        const std::string OnTextChanged        ("on-text-changed");           // Entry event selector
        const std::string OnTabChange          ("on-tab-change");             // Notebook event selector
        const std::string OnValueChanged       ("on-value-changed");          // SpinButton event selector
        const std::string OnToggle             ("on-toggle");                 // ToggleButton event selector
        const std::string OnCloseButton        ("on-close-button");           // Window event selector

        const std::string OnCreate             ("on-create");                 // this event dont need signal

        class SFGUI_API XMLLoader : protected ::tinyxml2::XMLDocument {
            public:
            typedef std::shared_ptr<XMLLoader> Ptr; //!< Shared pointer.
		    typedef std::shared_ptr<const XMLLoader> PtrConst; //!< Shared pointer.

            enum class Whitespace {
                PRESERVE_WHITESPACE,
                COLLAPSE_WHITESPACE,
                PEDANTIC_WHITESPACE
            };

            static std::map<std::string, sfg::Signal::SignalID*> SIGNAL;

    		static Ptr Create(bool processEntities = true, Whitespace whitespaceMode = Whitespace::PRESERVE_WHITESPACE);

            bool loadFromFile(std::string fileName, sf::RenderWindow* render = nullptr);

            std::shared_ptr<sfg::Widget> getWidget();
            operator sfg::Widget::Ptr();


            protected:
            XMLLoader(bool processEntities, Whitespace whitespaceMode);

            private:
            sfg::Widget::Ptr m_widget;
            sf::RenderWindow* m_render;
            bool m_reload;

            bool parser();
        };
    }  // namespace ui
}  // namespace sfg