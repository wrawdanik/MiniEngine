
if(APPLE)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions" )
endif()


include_directories(
		${CMAKE_SOURCE_DIR}/includes/support/
		${CMAKE_SOURCE_DIR}/includes/support/utils
		${CMAKE_SOURCE_DIR}/includes/third_party/tiny_thread
		${CMAKE_SOURCE_DIR}/includes/third_party/tiny_mt
		${CMAKE_SOURCE_DIR}/includes/third_party/rdestl
		${CMAKE_SOURCE_DIR}/includes/third_party/concurrentqueue
		${CMAKE_SOURCE_DIR}/includes/render_system/common
		${CMAKE_SOURCE_DIR}/includes/tasks
		${CMAKE_SOURCE_DIR}/includes/application
		${CMAKE_SOURCE_DIR}/includes/application/${MINIENGINE_PLATFORM}
)



file(   
	 GLOB SOURCES 
	"*.cpp"	
)

add_executable(Performance_Test  ${SOURCES} generic.cpp generic.h)
target_link_libraries(Performance_Test Support Tasks RenderSystem)


