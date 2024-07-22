#pragma once

#include <utility>

#include <halcyon/utility/concepts.hpp>

// utility/enum_bits.hpp:
// Helper template class for bit manipulation with enums.
// Aims to keep the safety of scoped enums while taking out the
// annoying part of writing std::to_underlying() everywhere.

namespace hal
{
    // A bitset-like class for (scoped) enums defined as bit masks (i.e. 0b1, 0b10, 0b100...).
    template <typename Enum, typename Value = meta::underlying_type<Enum>>
        requires std::is_enum_v<Enum>
    class enum_bitmask
    {
    private:
        constexpr static Value reduce(std::initializer_list<Enum> e)
        {
            Value mask { 0 };

            for (const Enum value : e)
                mask |= static_cast<Value>(value);

            return mask;
        }

    public:
        // Default constructor, initializes to zero.
        constexpr enum_bitmask()
            : m_mask { 0 }
        {
        }

        // Construct from an integral value.
        // This is mostly used by Halcyon internally.
        constexpr enum_bitmask(Value v)
            : m_mask { v }
        {
        }

        // Construct from a single enum.
        constexpr enum_bitmask(Enum e)
            : enum_bitmask { e }
        {
        }

        // OR together multiple enums.
        constexpr enum_bitmask(std::initializer_list<Enum> il)
            : enum_bitmask { reduce(il) }
        {
        }

        constexpr Value mask() const
        {
            return m_mask;
        }

        // Bitwise operators.

        constexpr enum_bitmask operator&(enum_bitmask b) const
        {
            return mask() & b.mask();
        }

        constexpr enum_bitmask operator|(enum_bitmask b) const
        {
            return mask() | b.mask();
        }

        constexpr enum_bitmask operator^(enum_bitmask b) const
        {
            return mask() ^ b.mask();
        }

        constexpr bool operator[](enum_bitmask e) const
        {
            return static_cast<bool>(mask() & e.mask());
        }

        constexpr enum_bitmask& operator+=(enum_bitmask e)
        {
            m_mask |= e.mask();
            return *this;
        }

        constexpr enum_bitmask& operator-=(enum_bitmask e)
        {
            m_mask &= ~static_cast<Value>(e.mask());
            return *this;
        }

        constexpr enum_bitmask& operator^=(enum_bitmask e)
        {
            m_mask ^= static_cast<Value>(e.mask());
            return *this;
        }

        constexpr auto operator<=>(const enum_bitmask&) const = default;

    private:
        Value m_mask;
    };
}
