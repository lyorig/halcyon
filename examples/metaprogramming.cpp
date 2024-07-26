#include <ctime>
#include <iostream>
#include <variant>

#include <halcyon/utility/metaprogramming.hpp>

// metaprogramming.cpp:
// Showcase of Halcyon metaprogramming.

// Shortcut for type equality check.
template <typename L, typename R>
constexpr inline bool eq = std::is_same_v<L, R>;

int main(int, char*[])
{
    // Check that you have successfully survived SDL's shenanigans.
    static_assert(hal::meta::is_correct_main<main>);

    using info_main  = hal::meta::func_info<decltype(main)>;
    using info_qsort = hal::meta::func_info<decltype(std::qsort)>;

    using joined = hal::meta::join<info_main::args, info_qsort::args>;
    static_assert(joined::size == info_main::args::size + info_qsort::args::size);

    using variant = joined::wrap<std::variant>;

    variant var;

    if (std::time(nullptr) % 2 == 0)
        var = 69;

    else
        var.emplace<joined::back>(nullptr);

    std::cout << "Variant holds a " << (std::holds_alternative<int>(var) ? "number" : "pointer") << '\n';

    return EXIT_SUCCESS;
}
