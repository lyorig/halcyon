project('Halcyon', 'cpp', default_options: ['cpp_std=c++23'])

sources = files(
  'src/audio/device.cpp',
  'src/audio/spec.cpp',
  'src/audio/stream.cpp',
  'src/events/holder.cpp',
  'src/events/keyboard.cpp',
  'src/events/mouse.cpp',
  'src/internal/rwops.cpp',
  'src/internal/string.cpp',
  'src/types/color.cpp',
  'src/utility/timer.cpp',
  'src/video/display.cpp',
  'src/video/driver.cpp',
  'src/video/message_box.cpp',
  'src/video/renderer.cpp',
  'src/video/texture.cpp',
  'src/video/window.cpp',
  'src/audio.cpp',
  'src/context.cpp',
  'src/debug.cpp',
  'src/events.cpp',
  'src/image.cpp',
  'src/surface.cpp',
  'src/templates.cpp',
  'src/ttf.cpp',
  'src/video.cpp'
)

sdl2 = dependency('sdl2', required: true)
sdl2_image = dependency('sdl2_image', required: true)
sdl2_ttf = dependency('sdl2_ttf', required: true)

includes = include_directories('include')

halcyon_lib = library('Halcyon', sources,
  include_directories: includes,
  dependencies: [sdl2, sdl2_image, sdl2_ttf]
)

halcyon_dep = declare_dependency(
  link_with: halcyon_lib,
  include_directories: includes,
  dependencies: [sdl2, sdl2_image, sdl2_ttf]
)
