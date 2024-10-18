# cmake version
cmake_minimum_required(VERSION 3.16.3)
# project name
project(src)

# set c++ version
set(CMAKE_CXX_STANDARD 17)

# include dir add,split by<space>
# include_directories()

# link dir add
# link_directories()

# install cmd
install(DIRECTORY ./cppnet DESTINATION include)

# add source
add_subdirectory(${PROJECT_SOURCE_DIR}/test)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib)
