#pragma once

#include <compare>
#include <initializer_list>

#include <halcyon/utility/metaprogramming.hpp>

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

        template <typename OtherValue>
            requires(sizeof(OtherValue) <= sizeof(Value))
        constexpr enum_bitmask(enum_bitmask<Enum, OtherValue> eb)
            : enum_bitmask { static_cast<Value>(eb.mask()) }
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

        constexpr enum_bitmask operator~() const
        {
            return ~mask();
        }

        // Returns true if the bits match exactly.
        constexpr bool operator[](enum_bitmask e) const
        {
            return (mask() & e.mask()) == e.mask();
        }

        constexpr enum_bitmask& operator+=(enum_bitmask e)
        {
            m_mask |= e.mask();
            return *this;
        }

        constexpr enum_bitmask& operator-=(enum_bitmask e)
        {
            m_mask &= ~e.mask();
            return *this;
        }

        constexpr enum_bitmask& operator&=(enum_bitmask e)
        {
            m_mask &= e.mask();
            return *this;
        }

        constexpr enum_bitmask& operator|=(enum_bitmask e)
        {
            m_mask |= e.mask();
            return *this;
        }

        constexpr enum_bitmask& operator^=(enum_bitmask e)
        {
            m_mask ^= e.mask();
            return *this;
        }

        constexpr std::strong_ordering operator<=>(const enum_bitmask&) const = default;

        constexpr operator bool() const
        {
            return mask() != 0;
        }

    private:
        constexpr enum_bitmask(Value v)
            : m_mask { v }
        {
        }

        Value m_mask;
    };
}
