 

include_directories(
	${CMAKE_SOURCE_DIR}/includes/third_party/tiny_mt
	${CMAKE_SOURCE_DIR}/includes/third_party/rdestl 
	${CMAKE_SOURCE_DIR}/includes/third_party/catch 	
	${CMAKE_SOURCE_DIR}/third_party/concurrentqueue
	${CMAKE_SOURCE_DIR}/includes/support/utils
	${CMAKE_SOURCE_DIR}/includes/support
)
 

file(   
	 GLOB SOURCES 
	"*.cpp"	
)



add_executable(Support_Test  ${SOURCES})
target_link_libraries(Support_Test Support )


