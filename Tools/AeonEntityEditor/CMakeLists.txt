#------------------------------------------------------------------------------
# Aeon Entity Editor
#
# Part of the Aeon CMake Build System.
#------------------------------------------------------------------------------
project( AeonEntityEditor ) 

set(EntityEditor_EXECUTABLE ${EDITOR_EXECUTABLE_NAME})

#------------------------------------------------------------------------------
# BOOST
if(CHECK_BOOST EQUAL 1)
	find_package(Boost REQUIRED COMPONENTS system thread)
	link_directories ( ${Boost_LIBRARY_DIRS} )
	include_directories ( ${Boost_INCLUDE_DIRS} )
endif()
#------------------------------------------------------------------------------
# Aeon
include_directories(
	${CMAKE_SOURCE_DIR}/Aeon/Core/Include 
	${CMAKE_SOURCE_DIR}/Aeon/Core/Include/Aeon/Serialization
)
#------------------------------------------------------------------------------
# Sources
file(GLOB_RECURSE EntityEditor_SRCS ./Source/*.cpp)

#------------------------------------------------------------------------------
# Headers / Meta Object Compiler headers
file(GLOB EntityEditor_Common_Headers ${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h )
file(GLOB EntityEditor_PluginAPI_Headers  ${CMAKE_CURRENT_SOURCE_DIR}/Include/Plugin/*.h )

set( EntityEditor_MOC_HDRS
  ${EntityEditor_Common_Headers} 
  ${EntityEditor_Plugin_Headers}
)

include_directories ( 
	Include/
	${CMAKE_SOURCE_DIR}/Dependencies/qjson/include/
	${CMAKE_SOURCE_DIR}/Dependencies/qtpropertybrowser/src/
	${CMAKE_SOURCE_DIR}/Dependencies/protobuf/src
	# Moveme
	${CMAKE_CURRENT_SOURCE_DIR}/../EditorPlugins/AeonWorldEditor/Include/
)

# Moveme
file(GLOB EntityEditor_Plugin_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../EditorPlugins/AeonWorldEditor/Source/*.cpp )
#------------------------------------------------------------------------------
# UI Files
file(GLOB EntityEditor_UIS ./UI/*.ui )
  
#------------------------------------------------------------------------------
# Additional settings and resources
SET( EntityEditor_RCS
   ./Resources/EntityEditor_QT.qrc
)
  
# enable warnings
#add_definitions( -Wall  )
  
# by default only QtCore and QtGui modules are enabled
# so enable additonal libs  
find_package( Qt4 REQUIRED)

#------------------------------------------------------------------------------
# add some useful macros and variables
# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
include( ${QT_USE_FILE} )
  
QT4_ADD_RESOURCES( EntityEditor_RC_SRC ${EntityEditor_RCS} )

# this will run uic on .ui files:
QT4_WRAP_UI( EntityEditor_UI_HDRS ${EntityEditor_UIS} )

# and finally this will run moc:
QT4_WRAP_CPP( EntityEditor_MOC_SRCS ${EntityEditor_MOC_HDRS})
  
# we need this to be able to include headers produced by uic in our code
# (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

if(MSVC)
SET ( RESOURCE_FILES
  "./resources/EntityEditor_Win32.rc"
  "./resources/EntityEditor_QT.qrc"
) 
endif()

if(WIN32)
	SET(LINK_LIBRARIES
		${CMAKE_SOURCE_DIR}/Dependencies/qjson/lib/
		${CMAKE_SOURCE_DIR}/Dependencies/qtpropertybrowser/lib/
		${CMAKE_SOURCE_DIR}/Dependencies/expat/lib/
	)

	# Use a version of protocol buffers specifically compiled for VS 2012, since
	# the VS 2010 versions they aren't forward compatible.
	if(MSVC11 EQUAL 1)
		SET(LINK_LIBRARIES
			${LINK_LIBRARIES}
			${CMAKE_SOURCE_DIR}/Dependencies/protobuf/lib/vs2012/
		)
	else()
		SET(LINK_LIBRARIES
			${LINK_LIBRARIES}
			${CMAKE_SOURCE_DIR}/Dependencies/protobuf/lib/
		)
	endif()

	link_directories(${LINK_LIBRARIES})
endif(WIN32)

#-----------------------------------------------------------------------------
# MSVC Project Source Grouping
SOURCE_GROUP("GeneratedFiles\\" FILES 
	${EntityEditor_MOC_SRCS} 
	${qjson_MOC_SRCS} 
	${EntityEditor_RC_SRC} 
	${EntityEditor_UI_HDRS} 
) 
SOURCE_GROUP("Resources\\" FILES ${RESOURCE_FILES})
SOURCE_GROUP("UI\\" FILES ${EntityEditor_UIS} )

# Moveme
SOURCE_GROUP("Plugins\\API" FILES ${EntityEditor_PluginAPI_Headers} )
SOURCE_GROUP("Plugins\\Include" FILES ${EntityEditor_Plugin_Headers} )
SOURCE_GROUP("Plugins\\Source" FILES ${EntityEditor_Plugin_SRC} )
#SOURCE_GROUP("Translations\\" FILES ${EntityEditor_Translations} )

#------------------------------------------------------------------------------
# Executable File and Linker directives
add_executable( 
	${EntityEditor_EXECUTABLE} 
	WIN32 MACOSX_BUNDLE 
	${EntityEditor_MOC_SRCS}
	${qjson_HDRS}
	${qjson_MOC_SRCS}
	${qjson_SRCS}
	${EntityEditor_SRCS} 	
	${EntityEditor_Plugin_SRC} 
	${EntityEditor_PluginAPI_Headers}
	${EntityEditor_Plugin_Headers} 
	${EntityEditor_RC_SRC} 
	${EntityEditor_UI_HDRS} 
	${EntityEditor_Common_Headers}
	${RESOURCE_FILES}
)
if(MSVC)
SET_TARGET_PROPERTIES(${EntityEditor_EXECUTABLE} 
	PROPERTIES
	COMPILE_FLAGS  "/MP"
)
endif()

SET(LIBS_TO_LINK
	${EntityEditor_EXECUTABLE}
	${QT_LIBRARIES} 
	${AEON_LIBRARY_NAME}
	${AEON_XML_LIBRARY_NAME}
)

if(WIN32)
	SET(LIBS_TO_LINK
		${LIBS_TO_LINK}
		optimized qjson debug qjson_d
		optimized libprotobuf debug libprotobuf_d
		optimized QtSolutions_PropertyBrowser-head debug QtSolutions_PropertyBrowser-headd
	)
elseif(UNIX)
	SET(LIBS_TO_LINK
		${LIBS_TO_LINK}
		qjson
		protobuf
		QtSolutions_PropertyBrowser-head
	)
endif()

if(CHECK_BOOST EQUAL 1 AND NOT MSVC)
	SET(LIBS_TO_LINK
		${LIBS_TO_LINK} 
		${Boost_LIBRARIES})
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11)
endif()

target_link_libraries(${LIBS_TO_LINK})
