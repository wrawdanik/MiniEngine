
MESSAGE( STATUS "Platform: " ${MINIENGINE_PLATFORM} )

include_directories(
	${CMAKE_SOURCE_DIR}/includes/support/
	${CMAKE_SOURCE_DIR}/includes/support/utils
	${CMAKE_SOURCE_DIR}/includes/third_party/tiny_thread
	${CMAKE_SOURCE_DIR}/includes/third_party/tiny_mt
	${CMAKE_SOURCE_DIR}/includes/third_party/rdestl
		${CMAKE_SOURCE_DIR}/includes/third_party/concurrentqueue
		${CMAKE_SOURCE_DIR}/includes/render_system/common
	${CMAKE_SOURCE_DIR}/includes/tasks
		${CMAKE_SOURCE_DIR}/includes/platform
		${CMAKE_SOURCE_DIR}/includes/platform/osx

)
 

file(   
	 GLOB SUPPORT_SOURCES 
	"${CMAKE_SOURCE_DIR}/source/third_party/tiny_thread/*.cpp"
	"${CMAKE_SOURCE_DIR}/source/third_party/tiny_mt/*.c"	
	"${CMAKE_SOURCE_DIR}/source/third_party/rdestl/*.cpp"		
	"${CMAKE_SOURCE_DIR}/source/support/*.cpp"
	"${CMAKE_SOURCE_DIR}/source/support/utils/*.cpp"
		"${CMAKE_SOURCE_DIR}/source/platform/*.cpp"
		"${CMAKE_SOURCE_DIR}/source/platform/osx/*.cpp"

		"${CMAKE_SOURCE_DIR}/includes/support/utils/*.h"
	"${CMAKE_SOURCE_DIR}/includes/support/*.h"
		"${CMAKE_SOURCE_DIR}/includes/platform/*.h"
		"${CMAKE_SOURCE_DIR}/includes/platform/osx/*.h"
)

 
#Generate the static library from the sources
add_library(Support STATIC ${SUPPORT_SOURCES} ../includes/support/utils/MiscUtils.h ../includes/support/utils/Range.h)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/libraries" CACHE PATH "default install path" FORCE )
endif()

 
MESSAGE( STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR} )
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX} ) 

MESSAGE( STATUS "TARGET_INSTALLNAME_DIR: " ${TARGET_INSTALLNAME_DIR} )


 
install(TARGETS Support DESTINATION ${MINIENGINE_PLATFORM}/${CMAKE_BUILD_TYPE})


file(   
	 GLOB TASKS_SOURCES 
	"${CMAKE_SOURCE_DIR}/source/tasks/*.cpp"	
	"${CMAKE_SOURCE_DIR}/includes/tasks/*.h"		
)

#Generate the static library from the sources
add_library(Tasks STATIC ${TASKS_SOURCES} )

 
install(TARGETS Tasks DESTINATION ${MINIENGINE_PLATFORM}/${CMAKE_BUILD_TYPE})


file(
		GLOB RENDER_SYSTEM_SOURCES
		"${CMAKE_SOURCE_DIR}/source/render_system/common/*.cpp"
		"${CMAKE_SOURCE_DIR}/includes/render_system/common/*.h"
)


add_library(RenderSystem STATIC ${RENDER_SYSTEM_SOURCES} ../includes/render_system/common/UserTaskQueue.h)


install(TARGETS RenderSystem DESTINATION ${MINIENGINE_PLATFORM}/${CMAKE_BUILD_TYPE})


file(
		GLOB PLATFORM_SOURCES
		"${CMAKE_SOURCE_DIR}/source/platform/${MINIENGINE_PLATFORM}/*.cpp"
		"${CMAKE_SOURCE_DIR}/source/platform/${MINIENGINE_PLATFORM}/*.mm"
		"${CMAKE_SOURCE_DIR}/includes/platform/${MINIENGINE_PLATFORM}/*.h"
)



add_library(Platform STATIC ${PLATFORM_SOURCES} ../includes/platform/Platform.h)


install(TARGETS RenderSystem DESTINATION ${MINIENGINE_PLATFORM}/${CMAKE_BUILD_TYPE})
