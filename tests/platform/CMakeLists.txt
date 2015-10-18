


include_directories(
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/tiny_mt
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/rdestl 
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/catch 	
	/Users/wrawdanik/Development/MiniEngine/includes/third_party/concurrentqueue
	/Users/wrawdanik/Development/MiniEngine/includes/support/utils
	/Users/wrawdanik/Development/MiniEngine/includes/support
		/Users/wrawdanik/Development/MiniEngine/includes/platform/${MINIENGINE_PLATFORM}
		/Users/wrawdanik/Development/MiniEngine/includes/platform
)


file(   
	 GLOB SOURCES 
	"*.cpp"	
)


SET(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework SystemConfiguration -framework CoreFoundation")


add_executable(Platform_Test  ${SOURCES} )
target_link_libraries(Platform_Test Platform Support Tasks)


