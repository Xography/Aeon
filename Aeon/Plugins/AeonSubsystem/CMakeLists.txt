#------------------------------------------------------------------------------
# Aeon Plugins
# Part of the Aeon CMake Build System.
#------------------------------------------------------------------------------
# Aeon Subsytem Build Configuration File
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# BOOST
if(CHECK_BOOST EQUAL 1)
	find_package(Boost REQUIRED COMPONENTS system thread)
	link_directories ( ${Boost_LIBRARY_DIRS} )
	include_directories ( ${Boost_INCLUDE_DIRS} )
	SET(Boost_USE_STATIC_LIBS ON)
endif()

#------------------------------------------------------------------------------
# General Setup
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/Include/
	${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Impl 
	${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Platform 
)

file(GLOB AeonSUBSYSTEM_GENERAL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/*.h)
file(GLOB AeonSUBSYSTEM_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Subsystem/*.h)
file(GLOB AeonSUBSYSTEM_IMPL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Impl/*.h)

SOURCE_GROUP("Header Files" FILES ${AeonSUBSYSTEM_GENERAL_HEADERS})
SOURCE_GROUP("Header Files\\Subsystem" FILES ${AeonSUBSYSTEM_HEADERS})
SOURCE_GROUP("Header Files\\Impl" FILES ${AeonSUBSYSTEM_IMPL_HEADERS})

file(GLOB AeonSUBSYSTEM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp)
file(GLOB AeonSUBSYSTEM_IMPL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/Impl/*.cpp)

SOURCE_GROUP("Source Files" FILES ${AeonSUBSYSTEM_SOURCES})
SOURCE_GROUP("Source Files\\Impl" FILES ${AeonSUBSYSTEM_IMPL_SOURCES})


IF(WIN32)
	file(GLOB  AeonSUBSYSTEM_PLATFORM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/Win32*.cpp)
	file(GLOB  AeonSUBSYSTEM_PLATFORM_HEADERS  ${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Platform/Win32*.h)
ELSE(WIN32)
	file(GLOB  AeonSUBSYSTEM_PLATFORM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/Unix*.cpp)
	file(GLOB  AeonSUBSYSTEM_PLATFORM_HEADERS  ${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Platform/Unix*.h)
ENDIF(WIN32) 

SOURCE_GROUP("Header Files\\Platform" FILES ${AeonSUBSYSTEM_PLATFORM_HEADERS})
SOURCE_GROUP("Source Files\\Platform" FILES ${AeonSUBSYSTEM_PLATFORM_SOURCES})

#------------------------------------------------------------------------------
SET(AeonSUBSYSTEM_HEADERS 
	${AeonSUBSYSTEM_HEADERS} 
	${AeonSUBSYSTEM_GENERAL_HEADERS}
	${AeonSUBSYSTEM_IMPL_HEADERS}
	${AeonSUBSYSTEM_PLATFORM_HEADERS}
)

SET(AeonSUBSYSTEM_SOURCES 
	${AeonSUBSYSTEM_SOURCES} 
	${AeonSUBSYSTEM_IMPL_SOURCES}
	${AeonSUBSYSTEM_PLATFORM_SOURCES}
)
#-------------------------------------------------------------------------------
if(WIN32)
	# Split here for easily adding future libraries.
	SET(LINK_LIBRARIES
		
	)

	# Use a version of protocol buffers specifically compiled for VS 2012, since
	# the VS 2010 versions they aren't forward compatible.
	if(MSVC11 EQUAL 1)
		SET(LINK_LIBRARIES
			${LINK_LIBRARIES}
			${CMAKE_SOURCE_DIR}/Dependencies/protobuf/lib/vs2012/
		)
	elseif(WIN32)
		SET(LINK_LIBRARIES
			${LINK_LIBRARIES}
			${CMAKE_SOURCE_DIR}/Dependencies/protobuf/lib/
		)
	endif()

	link_directories(${LINK_LIBRARIES})
endif()

#------------------------------------------------------------------------------
if(WIN32)
	if(AEON_STATIC)
		add_library(${AEON_SUBSYSTEM_LIBRARY_NAME} STATIC
			${AeonSUBSYSTEM_SOURCES}
			${AeonSUBSYSTEM_HEADERS}
		)
	else(AEON_STATIC)
		add_library(${AEON_SUBSYSTEM_LIBRARY_NAME} SHARED
			${AeonSUBSYSTEM_SOURCES}
			${AeonSUBSYSTEM_HEADERS}
		)
	endif(AEON_STATIC)
	
	SET_TARGET_PROPERTIES(${AEON_SUBSYSTEM_LIBRARY_NAME} 
		PROPERTIES
		COMPILE_FLAGS  "/MP"
	)
else(WIN32)
	add_library(${AEON_SUBSYSTEM_LIBRARY_NAME} SHARED
		${AeonSUBSYSTEM_SOURCES}
		${AeonSUBSYSTEM_HEADERS}
	)
endif(WIN32)
	
if (WIN32 AND NOT CYGWIN)
   set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../Lib)
endif(WIN32 AND NOT CYGWIN)

#---------------------------------------------------------------------------
if(CHECK_BOOST EQUAL 1 AND NOT MSVC)
	target_link_libraries (
		${AEON_SUBSYSTEM_LIBRARY_NAME}
		${AEON_LIBRARY_NAME}
		${Boost_LIBRARIES}
	)
else()
	target_link_libraries(${AEON_SUBSYSTEM_LIBRARY_NAME} ${AEON_LIBRARY_NAME})	
endif()
