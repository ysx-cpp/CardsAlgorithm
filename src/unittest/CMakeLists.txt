cmake_minimum_required (VERSION 3.2)
project("unittest")

include_directories(${PROJECT_SOURCE_DIR}../src)
include_directories(${PROJECT_SOURCE_DIR}../../googletest/googletest/include)

#message("${PROJECT_SOURCE_DIR}../googletest/googletest/include")

if(WIN32)
	# specify the C++ standard 
	set(CMAKE_CXX_STANDARD 11) 
	set(CMAKE_CXX_STANDARD_REQUIRED True)
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wpointer-arith -Wcast-qual -Wunreachable-code -Wredundant-decls -g -pipe -fPIC -std=c++11")
endif(WIN32)



aux_source_directory(. SRC)
list(APPEND SUB_SRCS ${SRC})

if(WIN32)
	file(GLOB HEAD ./*.h*)
	list(APPEND TARGET_HDR ${HEAD})
    add_executable(${PROJECT_NAME} ${SUB_SRCS} ${TARGET_HDR})
	#find_path(/home/ysx/CardsAlgorithm/build/lib)
	#find_library(gtest)
	set(LIBS gtest CardsAlgorithm)
	target_link_libraries(${PROJECT_NAME} ${LIBS})
else(WIN32)
	find_package(GTest REQUIRED)
    add_executable(${PROJECT_NAME} ${SUB_SRCS})
	set(LIBS pthread gtest CardsAlgorithm)
	target_link_libraries(${PROJECT_NAME} ${LIBS} ${GTest_LIBRARY})
endif(WIN32)
