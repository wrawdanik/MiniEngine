


include_directories(
	${CMAKE_SOURCE_DIR}/includes/third_party/tiny_mt
	${CMAKE_SOURCE_DIR}/includes/third_party/rdestl 
	${CMAKE_SOURCE_DIR}/includes/third_party/catch 	
	${CMAKE_SOURCE_DIR}/includes/third_party/concurrentqueue
	${CMAKE_SOURCE_DIR}/includes/support/utils
	${CMAKE_SOURCE_DIR}/includes/support
		${CMAKE_SOURCE_DIR}/includes/platform/${MINIENGINE_PLATFORM}
		${CMAKE_SOURCE_DIR}/includes/platform
)


file(   
	 GLOB SOURCES 
	"*.cpp"	
)

if(MINIENGINE_PLATFORM_OSX)
SET(CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework SystemConfiguration -framework CoreFoundation")
endif()

add_executable(Platform_Test  ${SOURCES} )
target_link_libraries(Platform_Test Platform Support )


