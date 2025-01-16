#pragma once

// internal/tags.hpp:
// Tag macros and shared tag names.

#define HAL_TAG_NAMESPACE      tag
#define HAL_TAG_IMPL_NAMESPACE impl

#define HAL_TAG_NAME(name) HAL_TAG_NAMESPACE ::HAL_TAG_IMPL_NAMESPACE::name

#define HAL_TAG(name)                                       \
    namespace tag                                           \
    {                                                       \
        namespace HAL_TAG_IMPL_NAMESPACE                    \
        {                                                   \
            struct name                                     \
            {                                               \
            };                                              \
        }                                                   \
        inline constexpr HAL_TAG_IMPL_NAMESPACE::name name; \
    }

namespace hal
{
    HAL_TAG(as_size)
    HAL_TAG(fill)
}
