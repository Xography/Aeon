#------------------------------------------------------------------------------
# Aeon Core Library
#
# Part of the Aeon CMake Build System.
#------------------------------------------------------------------------------
# Main Build Configuration File
#------------------------------------------------------------------------------
 include (GenerateExportHeader)
 
#------------------------------------------------------------------------------
# BOOST
if(CHECK_BOOST EQUAL 1)
	find_package(Boost REQUIRED COMPONENTS system thread )
	link_directories ( ${Boost_LIBRARY_DIRS} )
	include_directories ( ${Boost_INCLUDE_DIRS} )
	SET(Boost_USE_STATIC_LIBS ON)
endif()

#------------------------------------------------------------------------------
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/Include 
	${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Serialization
)

if(WIN32)
	include_directories(
		../../Dependencies/protobuf/src
	)
endif()

file(GLOB AeonCore_HEADERS 
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/*.h
)

file(GLOB AeonCore_MODULE_HEADERS 
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Modules/*.h
)

file(GLOB AeonCore_LOG_HEADERS 
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Logging/*.h
)

file(GLOB AeonCore_MEM_HEADERS 
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Memory/*.h
)

file(GLOB AeonCore_MULTICORE_HEADERS 
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Multicore/*.h
)

file(GLOB AeonCore_SERIALIZATION_HEADERS 
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Serialization/*.h
)

file(GLOB AeonCore_TYPES_HEADERS 
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Types/*.h
)

file(GLOB AeonCore_UTIL_HEADERS 
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Util/*.h
)
SOURCE_GROUP("Header Files\\" FILES ${AeonCore_HEADERS})
SOURCE_GROUP("Header Files\\Modules" FILES ${AeonCore_MODULE_HEADERS})
SOURCE_GROUP("Header Files\\Logging" FILES ${AeonCore_LOG_HEADERS})
SOURCE_GROUP("Header Files\\Memory" FILES ${AeonCore_MEM_HEADERS})
SOURCE_GROUP("Header Files\\Multicore" FILES ${AeonCore_MULTICORE_HEADERS})
SOURCE_GROUP("Header Files\\Types" FILES ${AeonCore_TYPES_HEADERS})
SOURCE_GROUP("Header Files\\Util" FILES ${AeonCore_UTIL_HEADERS})
SOURCE_GROUP("Generated Files\\IDL Headers" FILES ${AeonCore_SERIALIZATION_HEADERS})

file(GLOB AeonCore_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp)
file(GLOB AeonCore_LOG_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/Logging/*.cpp)
file(GLOB AeonCore_MEM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/Memory/*.cpp)
file(GLOB AeonCore_MULTICORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/Multicore/*.cpp)
file(GLOB AeonCore_MODULE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/Modules/*.cpp)
file(GLOB AeonCore_UTIL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/Util/*.cpp)
file(GLOB AeonCore_SERIALIZATION_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/Serialization/*.pb.cc)
file(GLOB AeonCore_TYPES_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Source/Types/*.cpp)

SOURCE_GROUP("Source Files\\" FILES ${AeonCore_SOURCES})
SOURCE_GROUP("Source Files\\Logging" FILES ${AeonCore_LOG_SOURCES})
SOURCE_GROUP("Source Files\\Memory" FILES ${AeonCore_MEM_SOURCES})
SOURCE_GROUP("Source Files\\Multicore" FILES ${AeonCore_MULTICORE_SOURCES})
SOURCE_GROUP("Source Files\\Modules" FILES ${AeonCore_MODULE_SOURCES})
SOURCE_GROUP("Source Files\\Types" FILES ${AeonCore_TYPES_SOURCES})
SOURCE_GROUP("Source Files\\Util" FILES ${AeonCore_UTIL_SOURCES})
SOURCE_GROUP("Generated Files\\IDL Sources" FILES ${AeonCore_SERIALIZATION_SOURCES})

file(GLOB AeonCore_STATIC_PLATFORM_HEADERS
		${CMAKE_CURRENT_SOURCE_DIR}/Include/Aeon/Platform/Platform.h
)

SET(AeonCore_PLATFORM_HEADERS ${AeonCore_STATIC_PLATFORM_HEADERS})
SOURCE_GROUP("Header Files\\Platform" FILES ${AeonCore_PLATFORM_HEADERS})
SOURCE_GROUP("Source Files\\Platform" FILES ${AeonCore_PLATFORM_SOURCES})
  
#------------------------------------------------------------------------------
SET(AeonCore_SOURCES
	${AeonCore_SOURCES} 
	${AeonCore_SUBSYS_SOURCES}
	${AeonCore_LOG_SOURCES}
	${AeonCore_MEM_SOURCES}
	${AeonCore_PLATFORM_SOURCES}
	${AeonCore_MULTICORE_SOURCES}
	${AeonCore_MODULE_SOURCES}
	${AeonCore_SERIALIZATION_SOURCES}
	${AeonCore_TYPES_SOURCES}
	${AeonCore_UTIL_SOURCES}
)
 
SET(AeonCore_HEADERS 
	${AeonCore_HEADERS}
	${AeonCore_MODULE_HEADERS}
	${AeonCore_SUBSYS_HEADERS}
	${AeonCore_LOG_HEADERS}
	${AeonCore_MEM_HEADERS}
	${AeonCore_PLATFORM_HEADERS}
	${AeonCore_MULTICORE_HEADERS}
	${AeonCore_SERIALIZATION_HEADERS}
	${AeonCore_TYPES_HEADERS}
	${AeonCore_UTIL_HEADERS}
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

#-------------------------------------------------------------------------------
# Library
if(AEON_STATIC)
	add_library(${AEON_LIBRARY_NAME} STATIC
		${AeonCore_SOURCES}
		${AeonCore_HEADERS}
	)
else()
	add_library(${AEON_LIBRARY_NAME}  SHARED
		${AeonCore_SOURCES}
		${AeonCore_HEADERS}
	)
endif()

if(WIN32 AND NOT CMAKE_COMPILER_IS_GNUCXX)
	SET_TARGET_PROPERTIES(${AEON_LIBRARY_NAME} 
		PROPERTIES
		COMPILE_FLAGS "/MP"
	)
endif()

if (WIN32 AND NOT CYGWIN)
   set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../Lib)
endif(WIN32 AND NOT CYGWIN)

if(CHECK_BOOST EQUAL 1 AND NOT MSVC)
target_link_libraries(${AEON_LIBRARY_NAME} ${Boost_LIBRARIES} )
endif()

if(WIN32)
	target_link_libraries(${AEON_LIBRARY_NAME} optimized libprotobuf debug libprotobuf_d)
else()
	target_link_libraries(${AEON_LIBRARY_NAME} protobuf rt)
endif()
	
#------------------------------------------------------------------------------
if(BUILD_TESTSUITE)
	SET(Boost_USE_STATIC_LIBS ON)

	file(GLOB AeonCore_Test_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Test/Include/*.h)
	file(GLOB AeonCore_Test_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Test/*.cpp)
	
	include_directories(
		${CMAKE_SOURCE_DIR}/Dependencies/igloo/
	)	
	
	SOURCE_GROUP("Tests" FILES ${AeonCore_Test_SOURCES})
	SOURCE_GROUP("Tests\\Include" FILES ${AeonCore_Test_HEADERS})

	#---------------------------------------------------------------------------
	add_executable(
		Test_Aeon
		#   WIN32
		MACOSX_BUNDLE
		${AeonCore_Test_HEADERS}
		${AeonCore_Test_SOURCES}
	) 
	
	add_definitions(-DBOOST_ALL_NO_LIB )
	target_link_libraries(Test_Aeon ${AEON_LIBRARY_NAME} ${Boost_LIBRARIES})
if(WIN32)
	target_link_libraries(Test_Aeon optimized libprotobuf debug libprotobuf_d)
else()
	target_link_libraries(Test_Aeon protobuf)
endif()

ENDIF(BUILD_TESTSUITE)
