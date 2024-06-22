#ifndef TEXTMAP_HPP
#define TEXTMAP_HPP

#include "consolearea.hpp"



class TextMap : public ConsoleArea
{
public:
    TextMap(Vector2u size)
        : ConsoleArea(size)
    {
        _map.resize(size.x * size.y);
    }

    void set_char(Vector2u pos, CharUnit&& ch) override
    {
        if (pos < size)
            _map[pos.x + pos.y * size.x] = std::move(ch);
    }

    CharUnit const& get_char(Vector2u pos) const override
    {
        if (pos < size)
            return _map[pos.x + pos.y * size.x];
        return CharUnit::none;
    }

public:
    static TextMap create_from(std::string const& tex);

public:
    TextMap strip() const;

protected:
    std::vector<CharUnit> _map;

};


class STDIOTextArea : public TextMap
{
public:
    using TextMap::TextMap;

public:
    std::string print() const;

};



#endif // TEXTMAP_HPP
