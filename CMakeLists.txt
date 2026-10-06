cmake_minimum_required(VERSION 3.28...4.3)

project(Halcyon VERSION 1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(HalcyonHelpers)

halcyon_setupbuildtype(Debug)

option(
    HALCYON_BUILD_TESTS
    "Build tests (see test/main.cpp) for use with CTest."
    OFF
)

option(
    HALCYON_BUILD_EXAMPLES
    "Build examples (see the examples/ directory)."
    OFF
)

option(
    HALCYON_WIN32_AUX_CONSOLE
    "Create an auxiliary console in debug mode on Windows."
    OFF
)

halcyon_createsourcelist(HALCYON_SOURCES src cpp
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

halcyon_createsourcelist(HALCYON_HEADERS include/halcyon hpp
    events/keyboard
    events/mouse
    events/variant
    internal/drawer
    internal/iostream
    internal/resource
    internal/tags
    internal/video_basic_types
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

target_sources(
    Halcyon
    PRIVATE
        FILE_SET SOURCES #
            BASE_DIRS src
            FILES ${HALCYON_SOURCES}
    PUBLIC
        FILE_SET HEADERS #
            BASE_DIRS include
            FILES ${HALCYON_HEADERS}
)

halcyon_setstandardwithoutextensions(Halcyon 23)

if(MSVC)
    target_compile_options(Halcyon PRIVATE /Wall)
else()
    target_compile_options(
        Halcyon
        PRIVATE -Wall -Wpedantic -Wextra -Wno-c++98-compat
    )
endif()

if(HALCYON_WIN32_AUX_CONSOLE)
    target_compile_definitions(Halcyon PRIVATE HAL_WIN32_AUX_CONSOLE)
endif()

option(HALCYON_SDL_VENDORED "Pull in SDL via CMake's `FetchContent` module" OFF)

if(HALCYON_SDL_VENDORED)
    include(FetchContent)
    FetchContent_Declare(
        SDL
        URL
            https://github.com/libsdl-org/SDL/releases/download/release-3.4.18/SDL3-3.4.18.tar.gz
    )

    FetchContent_Declare(
        SDL_image
        URL
            https://github.com/libsdl-org/SDL_image/releases/download/release-3.4.8/SDL3_image-3.4.8.tar.gz
    )

    FetchContent_Declare(
        SDL_ttf
        URL
            https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2.tar.gz
    )

    FetchContent_MakeAvailable(SDL SDL_image SDL_ttf)
else()
    # These are aliases for the shared/static variant.
    # You can link SDL statically by specifying `-DSDL_STATIC=ON` on the command line.
    find_package(SDL3 REQUIRED)
    find_package(SDL3_image REQUIRED)
    find_package(SDL3_ttf REQUIRED)
endif()

target_link_libraries(
    Halcyon
    PUBLIC SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf
)

option(BUILD_TESTING "Build the testing tree." OFF)

# Tests don't use a subdirectory (like examples), since they
# need access to internal headers.
if(BUILD_TESTING)
    include(CTest)

    add_executable(HalTest WIN32)
    target_sources(
        HalTest
        PRIVATE
            FILE_SET SOURCES #
                BASE_DIRS test
                FILES test/main.cpp
        PUBLIC
            FILE_SET HEADERS #
                BASE_DIRS test/include
                FILES test/include/data.hpp
    )

    target_link_libraries(HalTest PUBLIC Halcyon::Halcyon)

    halcyon_setstandardwithoutextensions(HalTest 23)

    set(ExeName "$<TARGET_FILE:HalTest>")

    function(addtest name cmd)
        add_test(NAME "${name}" COMMAND "${ExeName}" "${cmd}")
    endfunction()

    addtest(WindowResize --window-resize)
    addtest(BasicInit --basic-init)
    addtest(Clipboard --clipboard)
    addtest(SurfaceColor --surface-color)
    addtest(SurfaceCopy --surface-copy)
    addtest(Events --events)
    addtest(TTFInit --ttf-init)
    addtest(RValues --rvalues)
    addtest(Outputter --outputter)
    addtest(PngCheck --png-check)
    addtest(References --references)
    addtest(Shared --shared)
    addtest(Utilities --utilities)
    addtest(TextureManipulation --texture-manipulation)
    addtest(InvalidBuffer --invalid-buffer)
    addtest(InvalidTexture --invalid-texture)
    addtest(TextEngines --text-engines)

    # These tests rely on debug-mode asserts.
    add_test(NAME AssertFail COMMAND ${ExeName} --assert-fail)
    add_test(NAME InvalidEvent COMMAND ${ExeName} --invalid-event)

    set_tests_properties(AssertFail InvalidEvent PROPERTIES WILL_FAIL TRUE)
endif()

if(HALCYON_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Put compile commands somewhere clangd etc. can see them.
if(CMAKE_EXPORT_COMPILE_COMMANDS AND PROJECT_IS_TOP_LEVEL)
    cmake_path(APPEND LINK_FROM "${CMAKE_BINARY_DIR}" "compile_commands.json")
    cmake_path(
        APPEND LINK_TO
        "${CMAKE_SOURCE_DIR}"
        "build"
        "compile_commands.json"
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${LINK_FROM}" "${LINK_TO}"
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()
