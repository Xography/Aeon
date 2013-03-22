#------------------------------------------------------------------------------
# Aeon Plugins
# Part of the Aeon CMake Build System.
#------------------------------------------------------------------------------
# Aeon XML Build Configuration File
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# General Setup
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/Include 
	${CMAKE_SOURCE_DIR}/Dependencies/expat/include
	${CMAKE_CURRENT_SOURCE_DIR}/Test/include 
	${CMAKE_SOURCE_DIR}/Dependencies/igloo/
)
file(GLOB AeonXML_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/XML/*.h)
SOURCE_GROUP("Header Files" FILES ${AeonXML_HEADERS})

file(GLOB AeonXML_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp)

SOURCE_GROUP("Source Files" FILES ${AeonXML_SOURCES})

#------------------------------------------------------------------------------
if(WIN32)
	link_directories(
		${CMAKE_SOURCE_DIR}/Dependencies/expat/lib/
	)
endif(WIN32)
#------------------------------------------------------------------------------
if(WIN32)
	if(AEON_STATIC)
		add_library(${AEON_XML_LIBRARY_NAME} STATIC
			${AeonXML_SOURCES}
			${AeonXML_HEADERS}
		)
	else(AEON_STATIC)
		add_library(${AEON_XML_LIBRARY_NAME} SHARED
			${AeonXML_SOURCES}
			${AeonXML_HEADERS}
		)
	endif(AEON_STATIC)
	
	SET_TARGET_PROPERTIES(${AEON_XML_LIBRARY_NAME} 
		PROPERTIES
		COMPILE_FLAGS  "/MP"
	)
else(WIN32)
	add_library(${AEON_XML_LIBRARY_NAME} SHARED
		${AeonXML_SOURCES}
		${AeonXML_HEADERS}
	)
endif(WIN32)
	
if (WIN32 AND NOT CYGWIN)
   set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../Lib)
endif(WIN32 AND NOT CYGWIN)

target_link_libraries(${AEON_XML_LIBRARY_NAME} optimized expat debug expat_d)	
