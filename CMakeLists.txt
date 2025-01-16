# Halcyon CMake target file. add_subdirectory() and you should be good to go.

cmake_minimum_required(VERSION 3.20.0)

project(Halcyon VERSION 1.0 LANGUAGES CXX)

# Make sure SDL is present on the system.
find_package(SDL3 CONFIG COMPONENTS SDL3)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

option(BUILD_SHARED_LIBS
	"Build Halcyon as a shared library (.so/.dylib/.dll)."
	OFF
)

option(Halcyon_BUILD_TESTS
	"Build the test executable (in the test/ directory) for use with CTest."
	OFF
)

option(Halcyon_BUILD_EXAMPLES
	"Build examples (in the examples/ directory)."
	OFF
)

if (BUILD_SHARED_LIBS)
	message("Halcyon: Enabled building as shared library.")
endif()

# Sources.
set(Halcyon_SOURCES
    audio/device.cpp
    audio/spec.cpp
    audio/stream.cpp
    events/variant.cpp
    events/keyboard.cpp
    events/mouse.cpp
    internal/iostream.cpp
	internal/pixel_reference.cpp
    types/color.cpp
	types/result.cpp
    types/string.cpp
	utility/guard.cpp
    utility/timer.cpp
    video/display.cpp
    video/driver.cpp
    video/message_box.cpp
    video/renderer.cpp
    video/texture.cpp
    video/window.cpp
    audio.cpp
    debug.cpp
    events.cpp
    image.cpp
	library.cpp
	misc.cpp
	subsystem.cpp
    surface.cpp
	system.cpp
    templates.cpp
    ttf.cpp
    video.cpp
)

list(TRANSFORM Halcyon_SOURCES PREPEND src/)

# Create the library itself...
add_library(Halcyon)
add_library(Halcyon::Halcyon ALIAS Halcyon)

target_compile_features(Halcyon PUBLIC cxx_std_23)

if (MSVC)
	set(WarningFlags /Wall)
else()
	set(WarningFlags -Wall -Wpedantic)
endif()

# Might as well add these flags to everything.
add_compile_options(${WarningFlags})

target_include_directories(Halcyon PUBLIC include/)

target_sources(Halcyon PRIVATE
    ${Halcyon_SOURCES}
)

target_link_libraries(Halcyon PUBLIC
	$<IF:$<TARGET_EXISTS:SDL3::SDL3>,SDL3::SDL3,SDL3::SDL3-static>
	$<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image>,SDL3_image::SDL3_image,SDL3_image::SDL3_image-static>
    $<IF:$<TARGET_EXISTS:SDL3_ttf::SDL3_ttf>,SDL3_ttf::SDL3_ttf,SDL3_ttf::SDL3_ttf-static>
)

# Okay, that's it for the library.

# Testing.
if(Halcyon_BUILD_TESTS)
    message("Halcyon: Enabled tests.")

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
    add_test(NAME AudioInit COMMAND ${ExeName} --audio-init)
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
    message("Halcyon: Enabled examples.")

    set(EXAMPLE_SOURCES
        audio.cpp
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
