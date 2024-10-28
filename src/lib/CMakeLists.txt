# cmake version
cmake_minimum_required(VERSION 3.16.3)
# project name
project(lib)

# set c++ version
set(CMAKE_CXX_STANDARD 17)

# include dir add,split by<space>
include_directories(../cppnet)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	add_definitions(-DCPPNET_OPENSSL)
	aux_source_directory(../cppnet/ssl DIR_SRCS)
	link_libraries(-lssl -lcrypto)
endif()

# for local
include_directories(/opt/homebrew/Cellar/openssl@3/3.3.2/include)
link_directories(/opt/homebrew/Cellar/openssl@3/3.3.2/lib)
add_definitions(-DCPPNET_OPENSSL)
aux_source_directory(../cppnet/ssl DIR_SRCS)
link_libraries(-lssl -lcrypto)

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

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_library(cppnet STATIC ${DIR_SRCS})
