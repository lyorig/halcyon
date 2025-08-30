#pragma once

#include <halcyon/utility/metaprogramming.hpp>

#include <bit>
#include <cstdint>
#include <initializer_list>

// utility/enum_bits.hpp:
// Helper template class for bit manipulation with enums.
// Aims to keep the safety of scoped enums while taking out the
// annoying part of writing std::to_underlying() everywhere.

namespace hal
{
    // A bitset-like class for (scoped) enums defined as bit masks (i.e. 0b1, 0b10, 0b100...).
    // Has restricted functionality compared to pure ints to prevent excessive foot-shooting.
    template <typename Enum, typename Value = meta::underlying_type<Enum>>
        requires std::is_enum_v<Enum>
    class enum_bitmask
    {
        constexpr static Value reduce(std::initializer_list<Enum> e)
        {
            Value mask { 0 };

            for (const Enum v : e)
                mask |= static_cast<Value>(v);

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

        // Construct from an existing bitset. Its 'Value' type must be <= in size.
        template <typename OtherValue>
            requires(sizeof(OtherValue) <= sizeof(Value))
        constexpr enum_bitmask(enum_bitmask<Enum, OtherValue> eb)
            : enum_bitmask { static_cast<Value>(eb.mask()) }
        {
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

        // In-place bitwise operators.

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

        constexpr bool operator==(enum_bitmask e) const
        {
            return mask() == e.mask();
        }

        // Returns true if any of the bits set in `e` are set here.
        // This is useful when working with combined enum flags, i.e.
        // to check whether either left or right shift has been pressed.
        constexpr bool any(enum_bitmask e) const
        {
            return operator&(e).mask() != 0;
        }

        // Returns true if all bits set in 'e' are set here.
        constexpr bool all(enum_bitmask e) const
        {
            return operator&(e).mask() == e.mask();
        }

        constexpr bool operator[](enum_bitmask e) const
        {
            return all(e);
        }

        constexpr Value mask() const
        {
            return m_mask;
        }

        constexpr std::uint8_t count() const
        {
            return static_cast<std::uint8_t>(std::popcount(mask()));
        }

    private:
        constexpr enum_bitmask(Value v)
            : m_mask { v }
        {
        }

        Value m_mask;
    };
}
