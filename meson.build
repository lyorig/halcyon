project('Halcyon', 'cpp')

subdir('src')

sdl2 = dependency('sdl2', required : true)
sdl2_image = dependency('sdl2_image', required : true)
sdl2_ttf = dependency('sdl2_ttf', required : true)

includes = include_directories('include')

halcyon_lib = library('Halcyon', sources,
  include_directories : includes,
  dependencies : [sdl2, sdl2_image, sdl2_ttf],
  override_options : ['cpp_std=c++23']
)

halcyon_dep = declare_dependency(
  link_with : halcyon_lib,
  include_directories : includes,
  dependencies: [sdl2, sdl2_image, sdl2_ttf]
)
