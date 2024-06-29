#ifndef UTILS_H
#define UTILS_H


#include <stddef.h>
#include <string.h>


#ifdef __cplusplus


struct StringView
{
    const char* _data;
    size_t _size;

    const char* data() const noexcept { return _data; }
    size_t size() const noexcept { return _size; }

    const char* begin() const noexcept { return data(); }
    const char* end() const noexcept { return data() + size(); }

};


template<typename... A>
struct MyTuple
{
    static_assert(sizeof...(A) * 0, "Error instantiation");
};

template<typename First, typename... Types>
struct MyTuple<First, Types...>
{
    using ThisContainer = MyTuple<First, Types...>;
    using NextContainer = MyTuple<Types...>;
    using type = First;
    static constexpr size_t next_offset
       = offsetof(ThisContainer, next);
    First value;
    NextContainer next;
};

template<>
struct MyTuple<>
{};


template<typename Tuple, size_t _offset>
struct TupleFinderContext
{
    using type = typename Tuple::type;
    using container = Tuple;
    static constexpr size_t offset = _offset;
};

template<typename Original, typename Context, size_t index>
struct TupleContainerFinderImpl
   : TupleContainerFinderImpl<
        Original,
        TupleFinderContext<typename Context::container::NextContainer,
                           Context::offset
                            + Context::container::next_offset>,
                           index - 1>
{
    static constexpr typename
    TupleContainerFinderImpl<
        Original,
        TupleFinderContext<typename Context::container::NextContainer,
                           Context::offset
                            + Context::container::next_offset>,
                            index - 1>::container::type&
        get(typename Context::container& tuple)
    {
        return
            TupleContainerFinderImpl<
            Original,
            TupleFinderContext<typename Context::container::NextContainer,
                            Context::offset
                                + Context::container::next_offset>,
                            index - 1>::get(tuple.next);
    }
};


template<typename Original, typename Context>
struct TupleContainerFinderImpl<Original, Context, 0>
{
    static constexpr size_t offset
       = Context::offset;
    using container = typename Context::container;
    static constexpr typename Context::type&
        get(typename Context::container& tuple)
    {
        return tuple.value;
    }
};

template<typename Tuple, size_t index>
using FindTupleContainer
   = TupleContainerFinderImpl<
         Tuple,
         TupleFinderContext<Tuple, 0>,
         index>;


template<size_t index, typename Tuple>
constexpr size_t get_offset_of()
{
    using info = FindTupleContainer<Tuple, index>;
    return info::offset;
}

template<size_t index, typename Tuple>
constexpr typename FindTupleContainer<Tuple, index>::container::type& get(Tuple& tuple)
{
    using info = FindTupleContainer<Tuple, index>;
    return info::get(tuple);
}

template<size_t _index, typename Tuple>
struct tuple_element
{
    using type = typename FindTupleContainer<Tuple, _index>::container::type;
    static constexpr size_t index = _index;
    static constexpr size_t offset = FindTupleContainer<Tuple, index>::offset;
};



///////////////////////////////////////////////////////////////////////////////////////////////////////
// https://stackoverflow.com/questions/76302127/c11-equivalent-of-stdapply-plus-how-to-do-it-on-member-functions
// INVOKE -> invoke
// +apply(&A::f, A*, ...)
// std::tuple -> MyTuple
#include <utility>

namespace std14
{
    template<typename T, T... Ints>
    struct integer_sequence
    {
        typedef T value_type;
        static constexpr std::size_t size() { return sizeof...(Ints); }
    };

    template<std::size_t... Ints>
    using index_sequence = integer_sequence<std::size_t, Ints...>;

    template<typename T, std::size_t N, T... Is>
    struct make_integer_sequence : make_integer_sequence<T, N-1, N-1, Is...> {};

    template<typename T, T... Is>
    struct make_integer_sequence<T, 0, Is...> : integer_sequence<T, Is...> {};

    template<std::size_t N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;

    template<typename... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;
}


template <class F, class... Args>
constexpr auto invoke(F&& f, Args&&... args) ->
    decltype(std::forward<F>(f)(std::forward<Args...>(args)...))
{
      return std::forward<F>(f)(std::forward<Args...>(args)...);
}

template<typename Fptr, typename Class, typename... Args>
constexpr auto invoke(Fptr fptr, Class&& obj, Args&& ...args)
-> decltype((std::forward<Class>(obj).*fptr)(std::forward<Args>(args)...))
{
    return (std::forward<Class>(obj).*fptr)(std::forward<Args>(args)...);
}


template<typename F, typename Tuple, std::size_t... I>
constexpr auto apply_impl(F&& f, Tuple& t, std14::index_sequence<I...>)
-> decltype(invoke(std::forward<F>(f), get<I>(t)...))
{
    return invoke(std::forward<F>(f), get<I>(t)...);
}


template<typename F, typename Cls, typename Tuple, std::size_t... I>
constexpr auto apply_impl(F&& f, Cls* ptr, Tuple& t, std14::index_sequence<I...>)
-> decltype(invoke(std::forward<F>(f), std::forward<Cls>(*ptr), get<I>(t)...))
{
    return invoke(std::forward<F>(f), std::forward<Cls>(*ptr), get<I>(t)...);
}



template<typename F, typename... Args>
constexpr auto apply(F&& f, MyTuple<Args...>& tup)
-> decltype(apply_impl(
        std::forward<F>(f),
        tup,
        std14::make_index_sequence<sizeof...(Args)>{}
    ))
{
    return apply_impl(
        std::forward<F>(f),
        tup,
        std14::make_index_sequence<sizeof...(Args)>{}
    );
}

template<typename F, typename Cls, typename... Args>
constexpr auto apply(F&& f, Cls* ptr, MyTuple<Args...>& tup)
-> decltype(apply_impl(
        std::forward<F>(f),
        ptr,
        tup,
        std14::make_index_sequence<sizeof...(Args)>{}
    ))
{
    return apply_impl(
        std::forward<F>(f),
        ptr,
        tup,
        std14::make_index_sequence<sizeof...(Args)>{}
    );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////



#endif


#endif // UTILS_H
