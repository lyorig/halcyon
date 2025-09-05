#pragma once

#include <type_traits>

// utility/metaprogramming.hpp:
// Template metaprogramming. Almost everything here resides in namespace hal::meta.

namespace hal
{
    template <typename Return_type, typename... Args>
    using func_ptr = Return_type (*)(Args...);

    template <typename Return_type, typename... Args>
    using func_ref = Return_type (&)(Args...);

    namespace detail
    {
        // Primary template.
        template <std::size_t I, std::size_t N, typename... Rest>
        struct at;

        // Not found.
        template <std::size_t I, std::size_t N, typename Current, typename... Rest>
        struct at<I, N, Current, Rest...> : at<I + 1, N, Rest...>
        {
        };

        // Found.
        template <std::size_t N, typename Current, typename... Rest>
        struct at<N, N, Current, Rest...>
        {
            using type = Current;
        };

        // Primary template.
        template <std::size_t I, typename What, typename... Rest>
        struct find;

        // Not found.
        template <std::size_t I, typename What, typename Current, typename... Ts>
        struct find<I, What, Current, Ts...> : find<I + 1, What, Ts...>
        {
        };

        // Found.
        template <std::size_t I, typename What, typename... Ts>
        struct find<I, What, What, Ts...>
        {
            consteval static std::size_t value()
            {
                return I;
            }
        };

        template <typename>
        struct array_size;

        template <typename T, std::size_t N>
        struct array_size<T[N]>
        {
            consteval static std::size_t value()
            {
                return N;
            }
        };

        template <typename T>
        struct remove_pointer_to_const
        {
            using type = T;
        };

        template <typename T>
        struct remove_pointer_to_const<const T*>
        {
            using type = T*;
        };

        template <typename T, bool = std::is_enum_v<T>>
        struct underlying_type
        {
            using type = std::underlying_type_t<T>;
        };

        template <typename T>
        struct underlying_type<T, false>
        {
            using type = T;
        };
    }

    namespace meta
    {
        template <bool... Preds>
        constexpr inline bool all { (Preds && ...) };

        template <bool... Preds>
        constexpr inline bool any { (Preds || ...) };

        // Check whether a type is present in a parameter pack.
        template <typename What, typename... Where>
        constexpr inline bool is_present { (std::is_same_v<What, Where> || ...) };

        // Get the index at which a type resides in a parameter pack that contains it.
        // This will report the index of the first occurrence.
        template <typename What, typename... Where>
            requires is_present<What, Where...>
        constexpr inline std::size_t find { detail::find<0, What, Where...>::value() };

        // Check if a type is "innermost", as in, it is not a pointer, reference, and has no cv-qualifiers.
        template <typename T>
        constexpr inline bool is_innermost { !(std::is_const_v<T> || std::is_reference_v<T> || std::is_pointer_v<T> || std::is_volatile_v<T>)};

        template <typename T>
        concept innermost = is_innermost<T>;

        // Get the type residing at an index in a parameter pack.
        template <std::size_t I, typename... Ts>
            requires(I < sizeof...(Ts))
        using at = detail::at<0, I, Ts...>::type;

        template <typename T>
        constexpr inline std::size_t array_size { detail::array_size<T>::value() };

        template <typename T>
        using remove_pointer_to_const = detail::remove_pointer_to_const<T>::type;

        // Get the underlying type:
        //  - for enums, this returns the underlying type.
        //  - for everything else, this returns T.
        template <typename T>
        using underlying_type = detail::underlying_type<T>::type;

        // A holder, of sorts, of a parameter pack.
        // Provides basic functionality.
        template <typename... Ts>
        struct type_list
        {
            consteval static std::size_t size()
            {
                return sizeof...(Ts);
            }

            // Get the index of a type.
            template <typename T>
            consteval static std::size_t find()
            {
                return meta::find<T, Ts...>;
            }

            // Get the type at index N.
            template <std::size_t N>
            using at = meta::at<N, Ts...>;

            using front = at<0>;
            using back  = at<size() - 1>;

            // Wrap the provided type list in a type that takes a parameter pack.
            // Hopefully, that's understandable to you, 'cause it sure ain't to me.
            template <template <typename...> typename T>
            using wrap = T<Ts...>;
        };
    }

    namespace detail
    {
        template <typename...>
        struct join;

        template <typename... ArgsL, typename... ArgsR>
        struct join<meta::type_list<ArgsL...>, meta::type_list<ArgsR...>>
        {
            using type = meta::type_list<ArgsL..., ArgsR...>;
        };
    }

    namespace meta
    {
        template <typename Pack1, typename Pack2>
        using join = detail::join<Pack1, Pack2>::type;

        template <typename>
        struct func_info;

        // A neat way to get type information about a function.
        template <typename Ret, typename... Args>
        struct func_info<Ret(Args...)>
        {
            using function_type = Ret(Args...);
            using return_type   = Ret;
            using args          = type_list<Args...>;
        };

        // Adapter to get the info from a function pointer.
        template <typename Ret, typename... Args>
        struct func_info<func_ptr<Ret, Args...>> : func_info<Ret(Args...)>
        {
        };

        template <typename T>
        concept arithmetic = std::is_arithmetic_v<T> || std::is_enum_v<T>;

        template <typename T, typename... Ts>
        concept one_of = is_present<T, Ts...>;
    }

    namespace detail
    {
        template <auto, typename>
        struct struct_functor;

        template <auto Func, typename Ret, typename... Args>
        struct struct_functor<Func, Ret(Args...)>
        {
            static constexpr Ret operator()(Args... a)
            {
                return Func(a...);
            }
        };
    }

    namespace meta
    {
        template <auto Func>
        using struct_functor = detail::struct_functor<Func, std::remove_pointer_t<decltype(Func)>>;
    }
}
