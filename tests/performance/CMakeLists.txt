
if(APPLE)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions" )
endif()


include_directories(
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/tiny_thread
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/tiny_mt
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/rdestl 
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/catch 	
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/concurrentqueue
	/Users/wrawdanik/Development/MiniEngine/includes/support/utils
	/Users/wrawdanik/Development/MiniEngine/includes/support	
	/Users/wrawdanik/Development/MiniEngine/includes/jobs
		/Users/wrawdanik/Development/MiniEngine/includes/platform/osx
)
 

file(   
	 GLOB SOURCES 
	"*.cpp"	
)

add_executable(Performance_Test  ${SOURCES} generic.cpp generic.h)
target_link_libraries(Performance_Test Support Tasks)


