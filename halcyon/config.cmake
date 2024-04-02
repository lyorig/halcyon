include(${CMAKE_CURRENT_LIST_DIR}/lib/lyo/config.cmake)

set(HALCYON_SOURCES
cleanup.cpp
debug.cpp
events.cpp
image_loader.cpp
renderer.cpp
texture.cpp
ttf.cpp
window.cpp
surface.cpp
other/clipboard.cpp
other/display.cpp
other/driver.cpp
other/printing.cpp
other/system.cpp
)

list(TRANSFORM HALCYON_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/src/)

set(HALCYON_LIBRARIES
SDL2::SDL2main
SDL2::SDL2
SDL2_image::SDL2_image
SDL2_ttf::SDL2_ttf
)


find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)