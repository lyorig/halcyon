#pragma once

#include "halcyon/utility/metaprogramming.hpp"
#include <utility>

#include <halcyon/utility/concepts.hpp>

// utility/enum_bits.hpp:
// Helper template class for bit manipulation with enums.
// Aims to keep the safety of scoped enums while taking out the
// annoying part of writing std::to_underlying() everywhere.

namespace hal
{
    namespace detail
    {
        // OR together a set of values.
        // The name stands for "initializer list to bit mask".
        template <meta::arithmetic To, typename T>
            requires std::is_enum_v<T>
        constexpr To to_bitmask(std::initializer_list<T> list)
        {
            using ut = meta::underlying_type<To>;

            ut mask { 0 };

            for (const T value : list)
                mask |= static_cast<ut>(value);

            return static_cast<To>(mask);
        }

        template <meta::arithmetic To, typename T>
            requires std::is_enum_v<T>
        constexpr To to_bitset(std::initializer_list<T> list)
        {
            using ut = meta::underlying_type<To>;

            ut mask { 0 };

            for (const T value : list)
                mask |= (1 << static_cast<ut>(value));

            return static_cast<To>(mask);
        }

        template <typename Enum, typename Value>
            requires std::is_enum_v<Enum>
        class enum_bit_base
        {
        public:
            constexpr enum_bit_base()
                : m_mask { 0 }
            {
            }

            constexpr enum_bit_base(Value val)
                : m_mask { val }
            {
            }

            constexpr Value mask() const
            {
                return m_mask;
            }

            constexpr bool operator<=>(const enum_bit_base&) const = default;

        protected:
            Value m_mask;
        };
    }

    // A bitset-like class for (scoped) enums defined as bit masks (i.e. 0b1, 0b10, 0b100...).
    template <typename Enum, typename Value = meta::underlying_type<Enum>>
        requires std::is_enum_v<Enum>
    class enum_bitmask : public detail::enum_bit_base<Enum, Value>
    {
    private:
        using super = detail::enum_bit_base<Enum, Value>;

    public:
        using super::super;

        constexpr enum_bitmask(Enum e)
            : enum_bitmask { e }
        {
        }
        constexpr enum_bitmask(std::initializer_list<Enum> il)
            : super { detail::to_bitmask<Value>(il) }
        {
        }

        constexpr bool operator[](enum_bitmask e) const
        {
            return static_cast<bool>(super::mask() & e.mask());
        }

        constexpr enum_bitmask& operator+=(enum_bitmask e)
        {
            super::m_mask |= e.mask();
            return *this;
        }

        constexpr enum_bitmask& operator-=(enum_bitmask e)
        {
            super::m_mask &= ~static_cast<Value>(e.mask());
            return *this;
        }

        constexpr enum_bitmask& operator^=(enum_bitmask e)
        {
            super::m_mask ^= static_cast<Value>(e.mask());
            return *this;
        }
    };

    // A bitset-like class for (scoped) enums defined as indices (i.e. 1, 2, 3, 4...)
    template <typename Enum, typename Value = meta::underlying_type<Enum>>
        requires std::is_enum_v<Enum>
    class enum_bitset : public detail::enum_bit_base<Enum, Value>
    {
    private:
        using super = detail::enum_bit_base<Enum, Value>;

        constexpr static Value one { 1 };

    public:
        using super::super;

        constexpr enum_bitset(Enum e)
            : enum_bitset { e }
        {
        }

        constexpr enum_bitset(std::initializer_list<Enum> il)
            : super { detail::to_bitset<Value>(il) }
        {
        }

        constexpr bool operator[](enum_bitset e) const
        {
            return static_cast<bool>(super::mask() & e.mask());
        }

        constexpr enum_bitset& operator+=(enum_bitset e)
        {
            super::m_mask |= e.mask();
            return *this;
        }

        constexpr enum_bitset& operator-=(enum_bitset e)
        {
            super::m_mask &= ~e.mask();
            return *this;
        }

        constexpr enum_bitset& operator^=(enum_bitset e)
        {
            super::m_mask ^= e.mask();
            return *this;
        }
    };
}
