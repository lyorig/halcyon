cmake_minimum_required(VERSION 3.28.0)

project(Halcyon
    VERSION 1.0
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(HalcyonHelpers)

Halcyon_SetupBuildType(Debug)

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

Halcyon_CreateSourceList(HALCYON_SOURCES src cpp
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

Halcyon_CreateSourceList(HALCYON_HEADERS include/halcyon hpp
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

add_library(Halcyon)
add_library(Halcyon::Halcyon ALIAS Halcyon)

target_sources(Halcyon
    PRIVATE ${HALCYON_SOURCES}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES ${HALCYON_HEADERS}
)

Halcyon_SetStandardWithoutExtensions(Halcyon 23)

target_compile_options(Halcyon PRIVATE
    $<IF:$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>,/Wall,-Wall -Wpedantic -Wextra -Wno-c++98-compat>
)

if(HALCYON_WIN32_AUX_CONSOLE)
    target_compile_definitions(Halcyon PRIVATE HAL_WIN32_AUX_CONSOLE)
endif()

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)

target_link_libraries(Halcyon PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

# Tests don't use a subdirectory (like examples), since they
# need access to internal headers.
if(HALCYON_BUILD_TESTS)
    include(CTest)

    add_executable(HalTest WIN32 test/main.cpp)
    target_sources(HalTest
        PRIVATE
            test/main.cpp
        PUBLIC
            FILE_SET HEADERS
            BASE_DIRS test/include
            FILES test/include/data.hpp
    )

    target_link_libraries(HalTest PUBLIC Halcyon::Halcyon)

    Halcyon_SetStandardWithoutExtensions(HalTest 23)

    set(ExeName $<TARGET_FILE:HalTest>)

    function(AddTest name cmd)
        add_test(NAME "${name}" COMMAND "${ExeName}" "${cmd}")
    endfunction()

    AddTest(WindowResize --window-resize)
    AddTest(BasicInit --basic-init)
    AddTest(Clipboard --clipboard)
    AddTest(SurfaceColor --surface-color)
    AddTest(SurfaceCopy --surface-copy)
    AddTest(Events --events)
    AddTest(TTFInit --ttf-init)
    AddTest(RValues --rvalues)
    AddTest(Outputter --outputter)
    AddTest(PngCheck --png-check)
    AddTest(References --references)
    AddTest(Shared --shared)
    AddTest(Utilities --utilities)
    AddTest(TextureManipulation --texture-manipulation)
    AddTest(InvalidBuffer --invalid-buffer)
    AddTest(InvalidTexture --invalid-texture)
    AddTest(TextEngines --text-engines)

    # These tests rely on debug-mode asserts.
    if(CMAKE_BUILD_TYPE STREQUAL Debug)
        add_test(NAME AssertFail COMMAND ${ExeName} --assert-fail)
        add_test(NAME InvalidEvent COMMAND ${ExeName} --invalid-event)

        set_tests_properties(AssertFail InvalidEvent PROPERTIES WILL_FAIL TRUE)
    endif()
endif()

if(HALCYON_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
