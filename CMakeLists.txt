cmake_minimum_required( VERSION 3.19 )

set( library_name Ap4Decrypt )

project( ${library_name} CXX )

set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

if( UNIX AND NOT APPLE )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -maes" )
elseif( WIN32 )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX /Oi" )
endif()

include_directories( include )

file( GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/*.hpp )

set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${library_name} )

if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

add_executable( ${library_name} ${SRC_FILES} )

target_link_libraries( ${library_name} PRIVATE )

target_include_directories( ${library_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR} )
