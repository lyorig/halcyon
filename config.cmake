# Halcyon CMake configuration. include() this and you should be good to go.

include(FetchContent)

# Make sure SDL is present on the system.
find_package(SDL2       REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf   REQUIRED CONFIG)

# Include directores.
set(HALCYON_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include/)

# Sources.
set(HALCYON_SOURCES
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

list(TRANSFORM HALCYON_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)

# Libraries.
set(HALCYON_LIBRARIES
SDL2::SDL2main
SDL2::SDL2
SDL2_image::SDL2_image
SDL2_ttf::SDL2_ttf
)

# Halcyon uses C++23 features.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)