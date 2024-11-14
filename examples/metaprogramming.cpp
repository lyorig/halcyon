#include <ctime>
#include <iostream>
#include <variant>

#include <halcyon/utility/metaprogramming.hpp>

#include <halcyon/main.hpp>

// metaprogramming.cpp:
// Showcase of Halcyon metaprogramming.

// Shortcut for type equality check.
template <typename L, typename R>
constexpr inline bool eq = std::is_same_v<L, R>;

namespace
{
    // Using decltype(main) causes errors (on clang, at least).
    [[maybe_unused]] int main_like(int, char*[]);
}

int main(int, char*[])
{
    using info_main  = hal::meta::func_info<decltype(main_like)>;
    using info_qsort = hal::meta::func_info<decltype(std::qsort)>;

    using joined = hal::meta::join<info_main::args, info_qsort::args>;
    static_assert(joined::size() == info_main::args::size() + info_qsort::args::size());

    using variant = joined::wrap<std::variant>;

    variant var;

    if (std::time(nullptr) % 2 == 0)
        var = 69;

    else
        var.emplace<joined::back>(nullptr);

    std::cout << "Variant holds a " << (std::holds_alternative<int>(var) ? "number" : "pointer") << '\n';

    return EXIT_SUCCESS;
}
