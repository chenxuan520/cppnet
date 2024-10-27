# cmake version
cmake_minimum_required(VERSION 3.16.3)
# project name
project(test)

# set c++ version
set(CMAKE_CXX_STANDARD 17)

# include dir add,split by<space>
include_directories(../cppnet)
include_directories(../third_party/cpptest)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	add_definitions(-DCPPNET_OPENSSL)
	aux_source_directory(../cppnet/ssl DIR_SRCS)
	link_libraries(-lssl -lcrypto)
endif()

# for local
# include_directories(/opt/homebrew/Cellar/openssl@3/3.3.2/include)
# link_directories(/opt/homebrew/Cellar/openssl@3/3.3.2/lib)
# add_definitions(-DCPPNET_OPENSSL)
# aux_source_directory(../cppnet/ssl DIR_SRCS)
# link_libraries(-lssl -lcrypto)

# link dir add
link_libraries(-lpthread)

# add source
aux_source_directory(../cppnet/server/io_multiplexing DIR_SRCS)
aux_source_directory(../cppnet/server DIR_SRCS)
aux_source_directory(../cppnet/timer DIR_SRCS)
aux_source_directory(../cppnet/utils DIR_SRCS)
aux_source_directory(../cppnet/socket DIR_SRCS)

# add http source
file(GLOB HTTP_SRCS "../cppnet/http/*/*.cpp" "../cppnet/http/*/*/*.cpp")
list(APPEND DIR_SRCS ${HTTP_SRCS})

set(EXECUTABLE_OUTPUT_PATH ../../bin)

add_executable(test_bin ./main.cpp ${DIR_SRCS})

add_executable(link_bin ./main.cpp)
target_link_libraries(link_bin cppnet)
