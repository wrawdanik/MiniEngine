cmake_minimum_required(VERSION 3.0.0)
project(MiniEngine)



if(APPLE)
	add_definitions(-DMINIENGINE_PLATFORM_OSX)
	set (CMAKE_CXX_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden -std=c++11 -fno-exceptions" )
elseif(WIN32)	
	add_definitions(-DMINIENGINE_PLATFORM_WINDOWS)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	
endif()

SUBDIRS(source/) 
SUBDIRS(tests)

get_directory_property(${MINIENGINE_PLATFORM} DIRECTORY source VARIABLES MINIENGINE_PLATFORM)


MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )
MESSAGE( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER} )
MESSAGE( STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE} )
 
