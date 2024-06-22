#ifndef BUTTONWIDGET_HPP
#define BUTTONWIDGET_HPP

#include <memory>

#include "consolewidget.hpp"
#include "textmap.hpp"


class ConsoleButtonWidget : public ConsoleWidget
{
public:
    enum class State
    {
        Normal,
        Howered,
        Click,
        Inactive
    };

public:
    ConsoleButtonWidget()
        : _last_state(State::Normal) {}

public:
    void load(std::shared_ptr<TextMap>&& tex, State state);

public:
    void hower_event(bool enter) override;
    void key_event(int keycode, bool is_down) override;
    void draw(ConsoleArea* area, Vector2u size, bool resized) const override;

public:
    virtual void click_event() {}

public:
    std::vector<std::shared_ptr<TextMap>> _textures;
    State _last_state;

};



class ButtonProperty {};




#endif // BUTTONWIDGET_HPP
