#------------------------------------------------------------------------------
# Part of the Aeon CMake Build System.
#------------------------------------------------------------------------------
# Main Build Configuration File
#------------------------------------------------------------------------------

# Include some CMake functions
INCLUDE(CheckLibraryExists)
INCLUDE(CheckIncludeFile)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckCCompilerFlag)
INCLUDE(CheckCSourceCompiles)
#------------------------------------------------------------------------------
project(Aeon)


set (Boost_USE_STATIC_LIBS TRUE)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Lib)
 
cmake_minimum_required(VERSION 2.8)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

OPTION(BUILD_TESTSUITE "Build test suites for Aeon core and its plugins." OFF)
OPTION(BUILD_PLUGINS "Build plugins for the Aeon framework." ON)
OPTION(BUILD_AEON "Build the Aeon framework (as a library)." ON)
OPTION(BUILD_GAME "Build a sample game executable." ON)
OPTION(AEON_STATIC "Build Aeon statically (otherwise, create a dynamically linked library)." OFF)
OPTION(BUILD_QT_TOOLS "Build tools based on the Qt framework (i.e. the Aeon entity editor)." ON)

# VS2012 doesn't require boost since it ships with std::thread
IF(MSVC11 EQUAL 1 OR UNIX EQUAL 1)
	SET(CHECK_BOOST 0)
ELSE()
	SET(CHECK_BOOST 1)
ENDIF()	

IF(CMAKE_COMPILER_IS_GNUCXX)
	if(WIN32 EQUAL 1)
		SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
	ELSE()
		# Required on UNIX with GCC 4.7 to enable proper std::thread support.
		SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")
	ENDIF()

	# Append VC's _DEBUG define for debug builds. For the future: -Wall -Wextra -Wno-unused
	SET(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -g")
ELSEIF(UNIX EQUAL 1)
	message("To build on *NIX-based systems, you must use GCC 4.7 or higher!")
ENDIF()
	


#------------------------------------------------------------------------------
SET(AEON_LIBRARY_NAME Aeon) 
SET(AEON_XML_LIBRARY_NAME AeonXML)
SET(AEON_SUBSYSTEM_LIBRARY_NAME AeonSubsystem)
SET(AEON_COMPRESS_LIBRARY_NAME AeonCompress)
SET(GAME_LIBRARY_NAME GameCode)
SET(GAME_EXECUTABLE_NAME GameLauncher)
SET(EDITOR_EXECUTABLE_NAME Editor)

#------------------------------------------------------------------------------
include_directories(
	Aeon/Core/Include/
)
#------------------------------------------------------------------------------
IF(BUILD_AEON)
	ADD_SUBDIRECTORY(Aeon)
ENDIF(BUILD_AEON)

IF(BUILD_GAME)
	ADD_SUBDIRECTORY(AppLauncher)
	ADD_SUBDIRECTORY(AppDLL)
ENDIF(BUILD_GAME)

ADD_SUBDIRECTORY(Tools)

