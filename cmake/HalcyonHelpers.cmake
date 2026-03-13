# Set a target's `PUBLIC` C++ standard, and disable extensions.
function(set_std_no_ext TGT STD)
	target_compile_features("${TGT}" PUBLIC "cxx_std_${STD}")
	set_target_properties("${TGT}" PROPERTIES CXX_EXTENSIONS OFF)
endfunction()
