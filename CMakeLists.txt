cmake_minimum_required(VERSION 3.30.0)

project(Halcyon VERSION 1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(HalcyonHelpers)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

option(HALCYON_BUILD_TESTS
    "Build tests (see test/main.cpp) for use with CTest."
    OFF
)

option(HALCYON_BUILD_EXAMPLES
    "Build examples (see the examples/ directory)."
    OFF
)

option(HALCYON_WIN32_AUX_CONSOLE
    "Create an auxiliary console in debug mode on Windows."
    OFF
)

set(HALCYON_SOURCES
    events/keyboard
    events/mouse
    events/variant
    internal/iostream
    types/color
    types/string
    utility/guard
    utility/timer
    video/display
    video/driver
    video/message_box
    video/renderer
    video/texture
    video/window
    debug
    events
    filesystem
    hint
    image
    library
    misc
    properties
    subsystem
    surface
    system
    templates
    ttf
    video
)

list(TRANSFORM HALCYON_SOURCES PREPEND src/)
list(TRANSFORM HALCYON_SOURCES APPEND .cpp)

set(HALCYON_HEADERS
    events/keyboard
    events/mouse
    events/variant
    internal/drawer
    internal/iostream
    internal/resource
    internal/tags
    internal/video_basic_types
    types/c_string
    types/exception
    types/point
    types/rectangle
    types/result
    types/string
    utility/buffer
    utility/enum_bits
    utility/guard
    utility/metaprogramming
    utility/pass_key
    utility/printing
    utility/shared
    utility/strutil
    utility/timer
    video/display
    video/driver
    video/message_box
    video/palette
    video/renderer
    video/texture
    video/types
    video/window
    debug
    events
    filesystem
    hint
    image
    library
    main
    misc
    properties
    subsystem
    surface
    system
    ttf
    video
)

list(TRANSFORM HALCYON_HEADERS PREPEND include/halcyon/)
list(TRANSFORM HALCYON_HEADERS APPEND .hpp)

add_library(Halcyon)
add_library(Halcyon::Halcyon ALIAS Halcyon)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)

target_link_libraries(Halcyon PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

if (HALCYON_WIN32_AUX_CONSOLE)
    target_compile_definitions(Halcyon PRIVATE HAL_WIN32_AUX_CONSOLE)
endif()

set_std_no_ext(Halcyon 23)

target_compile_options(Halcyon PRIVATE
    $<IF:$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>,/Wall,-Wall -Wpedantic -Wextra -Wno-c++98-compat>
)

target_sources(Halcyon
    PRIVATE ${HALCYON_SOURCES}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES ${HALCYON_HEADERS}
)

# Tests don't use a subdirectory (like examples), since they
# need access to internal headers.
if(HALCYON_BUILD_TESTS)
    if (NOT CMAKE_BUILD_TYPE STREQUAL Debug)
        message(WARNING "Halcyon tests can only be built with `CMAKE_BUILD_TYPE=Debug` (current is `${CMAKE_BUILD_TYPE}`).")
    else()
        include(CTest)

        add_executable(HalTest WIN32 test/main.cpp)
        set_std_no_ext(HalTest 23)

        target_link_libraries(HalTest PUBLIC Halcyon::Halcyon)

        set(ExeName $<TARGET_FILE:HalTest>)

        add_test(NAME AssertFail COMMAND ${ExeName} --assert-fail)
        add_test(NAME WindowResize COMMAND ${ExeName} --window-resize)
        add_test(NAME BasicInit COMMAND ${ExeName} --basic-init)
        add_test(NAME Clipboard COMMAND ${ExeName} --clipboard)
        add_test(NAME SurfaceColor COMMAND ${ExeName} --surface-color)
        add_test(NAME SurfaceCopy COMMAND ${ExeName} --surface-copy)
        add_test(NAME Events COMMAND ${ExeName} --events)
        add_test(NAME TTFInit COMMAND ${ExeName} --ttf-init)
        add_test(NAME RValues COMMAND ${ExeName} --rvalues)
        add_test(NAME Outputter COMMAND ${ExeName} --outputter)
        add_test(NAME PngCheck COMMAND ${ExeName} --png-check)
        add_test(NAME References COMMAND ${ExeName} --references)
        add_test(NAME Shared COMMAND ${ExeName} --shared)
        add_test(NAME Utilities COMMAND ${ExeName} --utilities)
        add_test(NAME TextureManipulation COMMAND ${ExeName} --texture-manipulation)
        add_test(NAME InvalidBuffer COMMAND ${ExeName} --invalid-buffer)
        add_test(NAME InvalidTexture COMMAND ${ExeName} --invalid-texture)
        add_test(NAME InvalidEvent COMMAND ${ExeName} --invalid-event)
        add_test(NAME TextEngines COMMAND ${ExeName} --text-engines)

        set_tests_properties(AssertFail InvalidEvent PROPERTIES WILL_FAIL TRUE)
    endif()
endif()

if(HALCYON_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
