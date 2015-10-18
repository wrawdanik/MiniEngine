 

include_directories(
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/tiny_thread
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/tiny_mt
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/rdestl 
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/catch 	
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/concurrentqueue
	/Users/wrawdanik/Development/MiniEngine/includes/support/utils
	/Users/wrawdanik/Development/MiniEngine/includes/support	
	/Users/wrawdanik/Development/MiniEngine/includes/tasks
		/Users/wrawdanik/Development/MiniEngine/includes/platform/osx
)
 

file(   
	 GLOB SOURCES 
	"*.cpp"	
)

add_executable(Tasks_Test  ${SOURCES} Executor_Test.cpp )
target_link_libraries(Tasks_Test Support Tasks)


