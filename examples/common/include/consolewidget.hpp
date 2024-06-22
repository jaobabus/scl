#ifndef CONSOLEWIDGET_HPP
#define CONSOLEWIDGET_HPP

#include <vector>
#include <string>

#include "consolearea.hpp"



class ConsoleWidget;


class ConsoleAreaRect : public ConsoleArea
{
public:
    ConsoleAreaRect(ConsoleArea* view, Vector2u position, Vector2u size)
        : ConsoleArea(size), view(view), position(position) {}

public:
    void set_char(Vector2u pos,
                  CharUnit&& ch)
    {
        if (pos < size)
            view->set_char(pos + position, std::move(ch));
    }
    CharUnit const& get_char(Vector2u pos) const
    {
        if (pos < size)
            return view->get_char(pos + position);
        else
            return CharUnit::none;
    }

public:
    ConsoleArea* const view;
    Vector2u position;

};



class ConsoleWidgetView
{
public:
    ConsoleWidgetView(ConsoleWidget* widget, Vector2u position, Vector2u size)
        : widget(widget), position(position), size(size), _last_size(0) {}

public:
    void draw(ConsoleArea* area) const;

public:
    ConsoleWidget* const widget;
    Vector2u position;
    Vector2u size;

private:
    mutable Vector2u _last_size;

};



class ConsoleWidget
{
public:
    ConsoleWidget() {}

public:
    virtual void hower_event(bool enter) = 0;
    virtual void key_event(int keycode, bool is_down) = 0;
    virtual void draw(ConsoleArea* area, Vector2u size, bool resized) const = 0;

};


class ConsoleCanvasWidget : public ConsoleWidget
{
public:
    ConsoleCanvasWidget(ConsoleArea* area)
        : area(area) {}

public:
    void hower_event(bool enter) override {}
    void key_event(int keycode, bool is_down) override {}
    void draw(ConsoleArea* area, Vector2u size, bool resized) const override
    {
        area->draw(area);
    }

public:
    ConsoleArea* area;

};



class ConsoleZBuffer : public ConsoleWidget
{
public:
    ConsoleZBuffer() {}

public:
    void hower_event(bool enter) override {}
    void key_event(int keycode, bool is_down) override {}
    void draw(ConsoleArea* area, Vector2u size, bool resized) const override;

public:
    std::vector<ConsoleWidgetView*> views;

};


#endif // CONSOLEWIDGET_HPP
