# cmake version
cmake_minimum_required(VERSION 3.16.3)
# project name
project(temp)

# set c++ version
set(CMAKE_CXX_STANDARD 17)

set(ENABLE_SSL ON)
add_definitions(-DCPPNET_OPENSSL)
# link_libraries(-lssl -lcrypto)

add_subdirectory(../../src/lib cppnet)

# include dir add,split by<space>
include_directories(../../src/cppnet)

# link dir add
link_directories(../../src/lib)
# link_libraries(-lpthread)


# add source
add_executable(demo ./main.cpp)
target_link_libraries(demo cppnet)
target_link_libraries(demo cppnet)
