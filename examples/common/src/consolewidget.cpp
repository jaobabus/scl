#include <consolewidget.hpp>



void ConsoleWidgetView::draw(ConsoleArea* area) const
{
    ConsoleAreaRect rect(area, position, size);
    widget->draw(&rect, area->size, area->size != _last_size);
    _last_size = area->size;
}

void ConsoleZBuffer::draw(ConsoleArea* area, Vector2u size, bool resized) const
{
    for (auto view : views)
        view->draw(area);
}
