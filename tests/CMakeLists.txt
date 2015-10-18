
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions" )

if(APPLE)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions" )
endif()

SUBDIRS(support)
SUBDIRS(tasks)
SUBDIRS(performance)
SUBDIRS(platform)