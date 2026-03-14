# Set a target's `PUBLIC` C++ standard, and disable extensions.
function(Halcyon_SetStandardWithoutExtensions target standard)
	target_compile_features("${target}" PUBLIC "cxx_std_${standard}")
	set_target_properties("${target}" PROPERTIES CXX_EXTENSIONS OFF)
endfunction()

# Set `CMAKE_BUILD_TYPE` to a default value, if none is specified
# on the command line.
#
# Inspired by [Kitware's blog](https://www.kitware.com/cmake-and-the-default-build-type/).
function(Halcyon_SetupBuildType default)
	if(NOT CMAKE_BUILD_TYPE)
		if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
			set(CMAKE_BUILD_TYPE "${default}" CACHE STRING "Build type" FORCE)
			set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
				Debug Release MinSizeRel RelWithDebInfo)
		endif()
	endif()
endfunction()

# Create a list of file paths sharing a common parent directory and extension.
# This is used to simplify creation of source file lists, where
# you'd otherwise have to repeat the same directory prefix and
# file extension suffix for every element.
#
# For example, say you have a file structure:
# include/
#   my_project/
#     foo.hpp
#     bar.hpp
#     baz.hpp
#
# Instead of creating a list via:
# ```cmake
# set(MY_HEADERS
#   include/my_project/foo.hpp
#   include/my_project/bar.hpp
#   include/my_project/baz.hpp
# )
# ```
#
# you can use this function to achieve the same result:
# ```cmake
# Halcyon_CreateSourceList(MY_HEADERS include/my_project hpp
#   foo bar baz
# )
# ```
function(Halcyon_CreateSourceList result_var dir extension)
	set(${result_var} ${ARGN})
	list(TRANSFORM ${result_var} PREPEND "${dir}/")
	list(TRANSFORM ${result_var} APPEND ".${extension}")
	return(PROPAGATE ${result_var})
endfunction()
