cmake_minimum_required(VERSION 3.20)

project("Halcyon Examples" VERSION 1.0 LANGUAGES CXX)

function(setup NAME)
    add_executable(${NAME} WIN32 ${NAME}.cpp)
    target_compile_features(${NAME} PRIVATE cxx_std_23)
    target_link_libraries(${NAME} Halcyon::Halcyon)
    set_target_properties(${NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin/
    )
endfunction()

setup(events)
setup(invertor)
setup(message_box)
setup(metaprogramming)
setup(safety)
setup(text_render)
setup(update)
