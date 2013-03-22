#------------------------------------------------------------------------------
# Aeon Core Library
#
# Part of the Aeon CMake Build System.
#------------------------------------------------------------------------------
add_subdirectory(Core)
if(BUILD_PLUGINS)
	add_subdirectory(Plugins/AeonCompress)
	add_subdirectory(Plugins/AeonXML)
	add_subdirectory(Plugins/AeonSubsystem)
endif(BUILD_PLUGINS)