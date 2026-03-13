# Set a target's C++ standard, and disable extensions.
function(std_no_ext TGT STD)
	target_compile_features(${TGT} PUBLIC "cxx_std_${STD}")
	set_target_properties(${TGT} PROPERTIES CXX_EXTENSIONS OFF)
endfunction()
