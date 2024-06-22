#ifndef UTILS_HPP
#define UTILS_HPP


#include <stdint.h>


template<typename T>
struct Vector2
{
    Vector2()
        : Vector2(0, 0) {}
    explicit
    Vector2(T x)
        : Vector2(x, x) {}
    Vector2(T x, T y)
        : x(x), y(y) {}
    template<typename Ty>
    Vector2(Vector2<Ty> oth)
        : Vector2(oth.x, oth.y) {}

    T x, y;
};


using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned>;
using Vector2f = Vector2<float>;


#define DECLARE_VECTOR2_OPERATOR(op) \
template<typename T> \
inline \
Vector2<T> operator op(Vector2<T> left, Vector2<T> right) { \
    return Vector2<T>{ left.x op right.x, left.y op right.y }; \
}

#define DECLARE_VECTOR2_LOGIC_OPERATOR(op) \
template<typename T> \
inline \
bool operator op(Vector2<T> left, Vector2<T> right) { \
    return left.x op right.x and left.y op right.y ; \
}

DECLARE_VECTOR2_OPERATOR(+)
DECLARE_VECTOR2_OPERATOR(-)
DECLARE_VECTOR2_OPERATOR(/)
DECLARE_VECTOR2_OPERATOR(*)

DECLARE_VECTOR2_LOGIC_OPERATOR(<)
DECLARE_VECTOR2_LOGIC_OPERATOR(>)
DECLARE_VECTOR2_LOGIC_OPERATOR(<=)
DECLARE_VECTOR2_LOGIC_OPERATOR(>=)
DECLARE_VECTOR2_LOGIC_OPERATOR(==)
DECLARE_VECTOR2_LOGIC_OPERATOR(!=)


#endif // UTILS_HPP
