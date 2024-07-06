# Halcyon CMake target file. add_subdirectory() and you should be good to go.

project(Halcyon VERSION 1.0 LANGUAGES CXX)

# Make sure SDL is present on the system.
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

option(Halcyon_BUILD_TESTS "Build HalTest." FALSE)
option(Halcyon_BUILD_EXAMPLES "Build Halcyon Examples." FALSE)

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
    utility/strutil.cpp
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

# Halcyon uses C++23 features.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create the library itself...
add_library(Halcyon)

target_include_directories(Halcyon PUBLIC include/)

target_sources(Halcyon PRIVATE
    ${Halcyon_SOURCES}
)

unset(Halcyon_SOURCES)

target_link_libraries(Halcyon PUBLIC
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf)

# Okay, that's it for the library.

# Testing.
if(Halcyon_BUILD_TESTS)
    include(CTest)

    add_executable(HalTest test/main.cpp)

    target_link_libraries(HalTest PRIVATE Halcyon)

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
    add_test(NAME Views COMMAND ${ExeName} --views)
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
        message("Got ${TGT_NAME}")
        add_executable(${TGT_NAME} ${FILE} $<TARGET_OBJECTS:Halcyon>)
        target_link_libraries(${TGT_NAME} PRIVATE Halcyon)
        set_target_properties(${TGT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)
    endforeach()
endif()