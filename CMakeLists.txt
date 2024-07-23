# Halcyon CMake target file. add_subdirectory() and you should be good to go.

cmake_minimum_required(VERSION 3.20.0)

project(Halcyon VERSION 1.0 LANGUAGES CXX)

# Make sure SDL is present on the system.
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

option(Halcyon_BUILD_TESTS "Build HalTest.")
option(Halcyon_BUILD_EXAMPLES "Build Halcyon Examples.")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sources.
set(Halcyon_SOURCES
    audio/device.cpp
    audio/spec.cpp
    audio/stream.cpp
    events/holder.cpp
    events/keyboard.cpp
    events/mouse.cpp
    internal/rwops.cpp
    internal/string.cpp
    types/color.cpp
    utility/timer.cpp
    video/display.cpp
    video/driver.cpp
    video/message_box.cpp
    video/renderer.cpp
    video/texture.cpp
    video/window.cpp
    audio.cpp
    context.cpp
    debug.cpp
    events.cpp
    image.cpp
    surface.cpp
    templates.cpp
    ttf.cpp
    video.cpp
)

list(TRANSFORM Halcyon_SOURCES PREPEND src/)

# Create the library itself...
add_library(Halcyon)
add_library(Halcyon::Halcyon ALIAS Halcyon)

target_include_directories(Halcyon PUBLIC include/)

target_sources(Halcyon PRIVATE
    ${Halcyon_SOURCES}
)

target_link_libraries(Halcyon PUBLIC
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf)

# Okay, that's it for the library.

# Testing.
if(Halcyon_BUILD_TESTS)
    message("Halcyon: Enabled HalTest.")

    include(CTest)

    add_executable(HalTest WIN32 test/main.cpp)

	target_link_libraries(HalTest PUBLIC Halcyon)

    set(ExeName $<TARGET_FILE:HalTest>)

    add_test(NAME AssertFail COMMAND ${ExeName} --assert-fail)
    add_test(NAME WindowResize COMMAND ${ExeName} --window-resize)
    add_test(NAME BasicInit COMMAND ${ExeName} --basic-init)
    add_test(NAME Clipboard COMMAND ${ExeName} --clipboard)
    add_test(NAME SurfaceColor COMMAND ${ExeName} --surface-color)
    add_test(NAME Events COMMAND ${ExeName} --events)
    add_test(NAME TTFInit COMMAND ${ExeName} --ttf-init)
    add_test(NAME RValues COMMAND ${ExeName} --rvalues)
    add_test(NAME Scaler COMMAND ${ExeName} --scaler)
    add_test(NAME Outputter COMMAND ${ExeName} --outputter)
    add_test(NAME PngCheck COMMAND ${ExeName} --png-check)
    add_test(NAME References COMMAND ${ExeName} --references)
    add_test(NAME Metaprogramming COMMAND ${ExeName} --metaprogramming)
    add_test(NAME AudioInit COMMAND ${ExeName} --audio-init)
    add_test(NAME InvalidBuffer COMMAND ${ExeName} --invalid-buffer)
    add_test(NAME InvalidTexture COMMAND ${ExeName} --invalid-texture)
    add_test(NAME InvalidEvent COMMAND ${ExeName} --invalid-event)

    # Set which tests are expected to fail.
    set_tests_properties(AssertFail InvalidBuffer InvalidTexture InvalidEvent PROPERTIES WILL_FAIL TRUE)
endif()

# Examples.
if(Halcyon_BUILD_EXAMPLES)
    message("Halcyon: Enabled examples.")

    set(EXAMPLE_SOURCES
        audio.cpp
        invertor.cpp
        message_box.cpp
        metaprogramming.cpp
        safety.cpp
        events.cpp
        text_render.cpp
    )

    list(TRANSFORM EXAMPLE_SOURCES PREPEND examples/)

    foreach(FILE IN LISTS EXAMPLE_SOURCES)
        cmake_path(GET FILE STEM TGT_NAME)
        add_executable(${TGT_NAME} WIN32 ${FILE} $<TARGET_OBJECTS:Halcyon>)
        target_link_libraries(${TGT_NAME} PRIVATE Halcyon)
        set_target_properties(${TGT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
    endforeach()
endif()
