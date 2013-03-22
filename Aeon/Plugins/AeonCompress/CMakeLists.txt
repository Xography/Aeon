#------------------------------------------------------------------------------
# Aeon Plugins
# Part of the Aeon CMake Build System.
#------------------------------------------------------------------------------
# Aeon Compress Configuration File
#------------------------------------------------------------------------------

set(AEON_COMPRESS_LIBRARY_NAME AeonCompress)
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
	${CMAKE_CURRENT_SOURCE_DIR}/Include 
	${CMAKE_CURRENT_SOURCE_DIR}/Test/include 
	${CMAKE_SOURCE_DIR}/Dependencies/igloo/
)
file(GLOB AeonCompress_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Compress/*.h)
SOURCE_GROUP("Header Files" FILES ${AeonCompress_HEADERS})

file(GLOB AeonCompress_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp)

file(GLOB AeonCompress_MINIZIP_SOURCES 
	${CMAKE_SOURCE_DIR}/Dependencies/zlib/minizip/unzip.c
	${CMAKE_SOURCE_DIR}/Dependencies/zlib/minizip/ioapi.c
	${CMAKE_SOURCE_DIR}/Dependencies/zlib/minizip/zip.c
)

SOURCE_GROUP("Source Files" FILES ${AeonCompress_SOURCES})
SOURCE_GROUP("Source Files\\minizip" FILES ${AeonCompress_MINIZIP_SOURCES})

SET( AeonCompress_SOURCES
	${AeonCompress_SOURCES}
	${AeonCompress_MINIZIP_SOURCES}
)
#------------------------------------------------------------------------------
# ZLIB
    # We're statically compiling zlib 1.2.6 from source into the library 
	# on windows to reduce the external dependency count
	include_directories(AeonCompress 
		${CMAKE_SOURCE_DIR}/Dependencies/zlib/include 
		${CMAKE_SOURCE_DIR}/Dependencies/zlib/win32
		${CMAKE_SOURCE_DIR}/Dependencies/zlib/minizip
	)
if(WIN32)
	link_directories ( ${CMAKE_SOURCE_DIR}/Dependencies/zlib/lib  )
	SET(ZLIB_LIBRARY optimized zlibstatic debug zlibstaticd)
else()
	Find_Package ( ZLIB REQUIRED )
	link_directories ( ${ZLIB_LIBRARY_DIRS} )
	include_directories ( ${ZLIB_INCLUDE_DIRS} )
endif()

if(WIN32)
	if(AEON_STATIC)
		add_library(${AEON_COMPRESS_LIBRARY_NAME} STATIC
			${AeonCompress_SOURCES}
			${AeonCompress_HEADERS}
		)
	else(AEON_STATIC)
		add_library(${AEON_COMPRESS_LIBRARY_NAME} SHARED
			${AeonCompress_SOURCES}
			${AeonCompress_HEADERS}
		)
	endif()
	SET_TARGET_PROPERTIES(${AEON_COMPRESS_LIBRARY_NAME} 
		PROPERTIES
		COMPILE_FLAGS  "/MP"
	)
else(WIN32)
	add_library(${AEON_COMPRESS_LIBRARY_NAME} SHARED
		${AeonCompress_SOURCES}
		${AeonCompress_HEADERS}
	)
endif(WIN32)
if (WIN32 AND NOT CYGWIN)
   set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../Lib)
endif(WIN32 AND NOT CYGWIN)

if(CHECK_BOOST EQUAL 1)
	target_link_libraries(AeonCompress ${ZLIB_LIBRARY})
	if(NOT MSVC)
		target_link_libraries(AeonCompress ${Boost_LIBRARIES})
	endif()
else()
	target_link_libraries(AeonCompress ${ZLIB_LIBRARY})
endif()
	
#------------------------------------------------------------------------------
if(BUILD_TESTSUITE)
	file(GLOB AeonCompress_Test_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Test/Include/*.h)
	file(GLOB AeonCompress_Test_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Test/*.cpp)
	
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Test/Include )

	SOURCE_GROUP("Tests" FILES ${AeonCompress_Test_SOURCES})
	SOURCE_GROUP("Tests\\Include" FILES ${AeonCompress_Test_HEADERS})


	#---------------------------------------------------------------------------
	add_executable(
		Test_AeonCompress
		#   WIN32
		MACOSX_BUNDLE
		${AeonCompress_Test_SOURCES}
		${AeonCompress_Test_Headers}
	) 
	
	if(CMAKE_COMPILER_IS_GNUCXX)
	    add_definitions(-std=c++11)
	endif()
	
	#---------------------------------------------------------------------------
	if(CHECK_BOOST EQUAL 1)
		target_link_libraries (
			Test_AeonCompress
			${Boost_LIBRARIES}
			${AEON_COMPRESS_LIBRARY_NAME}
		)
	else()
		target_link_libraries (
			Test_AeonCompress
			${AEON_COMPRESS_LIBRARY_NAME}
		)
	endif()
ENDIF(BUILD_TESTSUITE)

