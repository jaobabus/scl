#include <textmap.hpp>

// :))
const CharUnit CharUnit::none = {L'\0'};



std::string rjump(Vector2i off)
{
    std::string result;
    if (off.x)
        result += "\033[" + std::to_string(abs(off.x)) + "CD"[off.x < 0];
    if (off.y)
        result += "\033[" + std::to_string(abs(off.y)) + "BA"[off.y < 0];
    return result;
}

std::string jump(Vector2i off)
{
    std::string result;
    if (off.x)
        result += "\033[" + std::to_string(abs(off.x)) + "CD"[off.x < 0];
    if (off.y)
        result += "\033[" + std::to_string(abs(off.y)) + "BA"[off.y < 0];
    return result;
}

std::string encode_utf8(wchar_t ch)
{
    if (ch > (wchar_t)127)
        return {L'?'};
    return {(char)ch};
}

std::string STDIOTextArea::print() const
{
    std::string result;
    result.reserve(sizeof("\033[38,5,255mS") * size.x * size.y);
    Vector2u last_pos{0, 0};
    for (unsigned y = 0; y < size.y; y++)
        for (unsigned x = 0; x < size.x; x++)
        {
            auto& unit = get_char({x, y});
            if (unit.ch == L'\0')
                continue;
            if (x != last_pos.x or y != last_pos.y)
                result += rjump(Vector2i(x, y) - Vector2i(last_pos));
            result += encode_utf8(unit.ch);
            last_pos = {x + 1, y};
        }
    return result;
}

TextMap TextMap::create_from(const std::string& tex)
{
    TextMap map{{240, 120}};
    ::draw(&map, {0, 0}, tex);
    return map.strip();
}

TextMap TextMap::strip() const
{
    Vector2i left_min{0x7F7F7FFF, 0x7F7F7FFF}, right_max{-1, -1};
    bool found = false;
    for (unsigned y = 0; y < size.y; y++)
        for (unsigned x = 0; x < size.x; x++)
        {
            if (get_char({x, y}).ch != L'\0') {
                if (x > right_max.x)
                    right_max.x = x;
                if (y > right_max.y)
                    right_max.y = y;
                if (x < left_min.x)
                    left_min.x = x;
                if (y < left_min.y)
                    left_min.y = y;
                found = true;
            }
        }
    if (found) {
        TextMap result{right_max - left_min};
        draw(&result);
        return result;
    }
    return *this;
}
