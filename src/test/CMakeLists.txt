# cmake version
cmake_minimum_required(VERSION 3.16.3)
# project name
project(test)

# set c++ version
set(CMAKE_CXX_STANDARD 17)

# include dir add,split by<space>
include_directories(../cppnet)
include_directories(../third_party/cpptest)

# link dir add
# link_directories()

# add source
aux_source_directory(../cppnet/server/io_multiplexing DIR_SRCS)
aux_source_directory(../cppnet/server DIR_SRCS)
aux_source_directory(../cppnet/timer DIR_SRCS)
aux_source_directory(../cppnet/utils DIR_SRCS)
aux_source_directory(../cppnet/socket DIR_SRCS)

set(EXECUTABLE_OUTPUT_PATH ../../bin)
add_executable(test_bin ./main.cpp ${DIR_SRCS})
