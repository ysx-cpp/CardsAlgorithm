cmake_minimum_required (VERSION 2.8)
set(TARGET_NAME "ChessCardsAgo")
project(${TARGET_NAME})

include_directories(${PROJECT_SOURCE_DIR}/src)

if(WIN32)
	# specify the C++ standard 
	set(CMAKE_CXX_STANDARD 11) 
	set(CMAKE_CXX_STANDARD_REQUIRED True)
else(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -pipe -fPIC -std=c++11")
endif(WIN32)

aux_source_directory(src/majong SRC1)
aux_source_directory(src/poker SRC2)
aux_source_directory(src/zipai SRC3)

list(APPEND TARGET_SRC ${SRC} ${SRC1} ${SRC2} ${SRC3})

if(WIN32)
	file(GLOB HEAD src/*.h*)
	file(GLOB HEAD1 src/majong/*.h*)
	file(GLOB HEAD2 src/poker/*.h*)
	file(GLOB HEAD3 src/zipai/*.h)
	list(APPEND TARGET_HDR ${HEAD} ${HEAD1} ${HEAD2} ${HEAD3})

    add_library(${TARGET_NAME} ${TARGET_SRC} ${TARGET_HDR})
	source_group("majong" FILES ${HEAD1} ${SRC1})
	source_group("poker" FILES ${HEAD2} ${SRC2})
	source_group("zipai" FILES ${HEAD3} ${SRC3})
else(WIN32)
    add_library(${TARGET_NAME} STATIC ${TARGET_SRC})
endif(WIN32)

