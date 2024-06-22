#include <buttonwidget.hpp>



void ConsoleButtonWidget::load(std::shared_ptr<TextMap>&& tex, State state)
{
    _textures.resize(std::max<size_t>(size_t(state), _textures.size()));
    _textures[size_t(state)] = std::move(tex);
}

void ConsoleButtonWidget::hower_event(bool enter)
{
    if (_last_state == State::Inactive)
        return;
    if ((_last_state == State::Howered) != enter)
    {

        _last_state = (enter ? State::Howered : State::Normal);
    }
}

void ConsoleButtonWidget::key_event(int keycode, bool is_down)
{
    if (keycode == '\n' and not is_down)
        click_event();
}

void ConsoleButtonWidget::draw(ConsoleArea* area, Vector2u size, bool resized) const
{
    if (_textures.size() < size_t(_last_state))
        _textures[size_t(_last_state)]->draw(area);
    else
        ::draw(area, {0, 0}, "<notex:" + std::to_string(size_t(_last_state)) + ">");
}
