#include <effect.hpp>

#include <vector>
#include <string.h>

std::vector<game::Effect> EFFECT_STACK;

static float FOG_INTENSITY = 0.0;
static sf::Color FOG_COLOR(0x0);
static sf::RectangleShape FOG_AREA;
static sf::Vector2f SHAKE_INITIAL_POSITION;
static int8_t SHAKE_DIRECTION = 1;
static bool SHAKE_INIT = false;

static void addEffect(game::effect::Type type, sf::Color color, float intensity, float seconds, sf::View *view){
    EFFECT_STACK.emplace_back(view, color, seconds, intensity, type);
}

namespace game{
    namespace effect{
        void fogIn(::sf::Color color, float seconds){
            addEffect(Type::FOG_IN, color, 1.0, seconds, nullptr);
        }
        void fogOut(float seconds){
            addEffect(Type::FOG_OUT, ::sf::Color(0x00), 1.0, seconds, nullptr);
        }
        void shake(::sf::View *view, float intensity, float seconds){
            addEffect(Type::SHAKE, ::sf::Color(0x00), intensity, seconds, view);
        }

        void render(::sf::RenderWindow &render){
            render.draw(FOG_AREA);

            if(EFFECT_STACK.size() == 0) return;
            Effect *effect = &EFFECT_STACK[0];

            switch(effect->type){
                case Type::FOG_IN:
                    if(effect->intensity > 1.0) effect->intensity = 1.0;

                    FOG_COLOR = effect->color;
                    FOG_INTENSITY += 1.0/effect->seconds/60;
                    if(FOG_INTENSITY >= effect->intensity){
                        FOG_INTENSITY = 1.0;
                        EFFECT_STACK.erase(EFFECT_STACK.begin());
                    }
                    FOG_COLOR.a = FOG_INTENSITY*255;
                    break;
                case Type::FOG_OUT:
                    FOG_INTENSITY -= 1.0/effect->seconds/60;
                    if(FOG_INTENSITY <= 0){
                        FOG_INTENSITY = 0;
                        EFFECT_STACK.erase(EFFECT_STACK.begin());
                    }
                    FOG_COLOR.a = FOG_INTENSITY*255;
                    break;
                case Type::SHAKE:
                    if(!SHAKE_INIT){
                        SHAKE_INITIAL_POSITION = effect->view->getCenter();
                    }
                    ::sf::Vector2f position(SHAKE_INITIAL_POSITION.x+(effect->intensity*SHAKE_DIRECTION), SHAKE_INITIAL_POSITION.y);
                    effect->view->setCenter(position);
                    SHAKE_DIRECTION *= -1;

                    effect->seconds -= 1.0/60.0;
                    if(effect->seconds <= 0){
                        effect->view->setCenter(SHAKE_INITIAL_POSITION);
                        EFFECT_STACK.erase(EFFECT_STACK.begin());
                    }
                    break;
            }

            FOG_AREA.setPosition({0, 0});
            FOG_AREA.setSize(::sf::Vector2f(render.getSize()));
            FOG_AREA.setFillColor(FOG_COLOR);

        }
        bool isLocked(){
            return (EFFECT_STACK.size() > 0);
        }
        void waitFor(){
            while(EFFECT_STACK.size()) sf::sleep(sf::milliseconds(500));
        }
    }

    Effect::Effect(::sf::View *view, ::sf::Color color, float seconds, float intensity, effect::Type type){
        this->type = type;
        this->view = view;
        this->color = color;
        this->seconds = seconds;
        this->intensity = intensity;
    }
}
