cmake_minimum_required(VERSION 3.20.0)

project(Halcyon VERSION 1.0 LANGUAGES CXX)

option(Halcyon_BUILD_TESTS
    "Build tests (see test/main.cpp) for use with CTest."
    OFF
)

option(Halcyon_BUILD_EXAMPLES
    "Build examples (see the examples/ directory)."
    OFF
)

option(Halcyon_WIN32_AUX_CONSOLE
    "Create an auxiliary console in debug mode on Windows."
    OFF
)

set(Halcyon_SOURCES
    events/variant
    events/keyboard
    events/mouse
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

list(TRANSFORM Halcyon_SOURCES PREPEND src/)
list(TRANSFORM Halcyon_SOURCES APPEND .cpp)

add_library(Halcyon)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)

target_link_libraries(Halcyon PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

if (Halcyon_WIN32_AUX_CONSOLE)
    target_compile_definitions(Halcyon PRIVATE HAL_WIN32_AUX_CONSOLE)
endif()

# Halcyon uses C++23.
target_compile_features(Halcyon PUBLIC
    cxx_std_23
)

target_compile_options(Halcyon PRIVATE
    $<IF:$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>,/Wall,-Wall -Wpedantic -Wextra -Wno-c++98-compat>
)

target_include_directories(Halcyon PUBLIC
    include/
)

target_sources(Halcyon PRIVATE
    ${Halcyon_SOURCES}
)

# Testing.
if(Halcyon_BUILD_TESTS)
    include(CTest)

    add_executable(HalTest WIN32 test/main.cpp)

    target_link_libraries(HalTest PUBLIC Halcyon)

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

    # Set which tests are expected to fail.
    set_tests_properties(AssertFail InvalidEvent PROPERTIES WILL_FAIL TRUE)
endif()

# Examples.
if(Halcyon_BUILD_EXAMPLES)
    set(EXAMPLE_SOURCES
        invertor.cpp
        message_box.cpp
        metaprogramming.cpp
        safety.cpp
        events.cpp
        text_render.cpp
        update.cpp
    )

    list(TRANSFORM EXAMPLE_SOURCES PREPEND examples/)

    foreach(FILE IN LISTS EXAMPLE_SOURCES)
        cmake_path(GET FILE STEM TGT_NAME)
        add_executable(${TGT_NAME} WIN32 ${FILE})
        target_link_libraries(${TGT_NAME} PRIVATE Halcyon::Halcyon)
        set_target_properties(${TGT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
    endforeach()
endif()

add_library(Halcyon::Halcyon ALIAS Halcyon)
