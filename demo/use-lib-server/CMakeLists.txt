# cmake version
cmake_minimum_required(VERSION 3.22.1)
# project name
project(use-lib-server)

# set c++ version
set(CMAKE_CXX_STANDARD 17)

# include dir add,split by<space>
include_directories(./cppnet/include/cppnet)

# link dir add
link_directories(./cppnet/lib)
link_libraries(-lcppnet)

# add source
add_executable(libserver main.cpp)
