# cmake version
cmake_minimum_required(VERSION 3.16.3)
# project name
project(test)

# set c++ version
set(CMAKE_CXX_STANDARD 17)

# include dir add,split by<space>
include_directories(../cppnet)
include_directories(../third_party/cpptest)

# default enable ssl
option(ENABLE_SSL "Enable SSL" on)

# ssl
if(ENABLE_SSL)
	if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
		# Ask Homebrew for the stable opt/ prefix instead of building a
		# Cellar/<version> path: versions with a revision suffix (e.g. 3.6.0_1)
		# made the old regex-derived path point at a missing directory.
		execute_process(
			COMMAND brew --prefix openssl@3
			OUTPUT_VARIABLE OPENSSL_PREFIX
			OUTPUT_STRIP_TRAILING_WHITESPACE
			RESULT_VARIABLE BREW_PREFIX_RESULT
		)
		if(NOT BREW_PREFIX_RESULT EQUAL 0 OR OPENSSL_PREFIX STREQUAL "")
			set(OPENSSL_PREFIX /opt/homebrew/opt/openssl@3)
		endif()
		message(STATUS "OpenSSL prefix: ${OPENSSL_PREFIX}")
		include_directories(${OPENSSL_PREFIX}/include)
		link_directories(${OPENSSL_PREFIX}/lib)
	endif()
	add_definitions(-DCPPNET_OPENSSL)
	link_libraries(-lssl -lcrypto)
endif()

# link dir add
link_libraries(-lpthread)
# if windows add -lwsock32
if(WIN32)
	link_libraries(-lws2_32)
endif()

# add source
aux_source_directory(../cppnet/server/io_multiplexing DIR_SRCS)
aux_source_directory(../cppnet/server DIR_SRCS)
aux_source_directory(../cppnet/timer DIR_SRCS)
aux_source_directory(../cppnet/utils DIR_SRCS)
aux_source_directory(../cppnet/socket DIR_SRCS)
aux_source_directory(../cppnet/log DIR_SRCS)
aux_source_directory(../cppnet/ssl DIR_SRCS)

# add http source
file(GLOB HTTP_SRCS "../cppnet/http/*/*.cpp" "../cppnet/http/*/*/*.cpp")
list(APPEND DIR_SRCS ${HTTP_SRCS})

# record git hash version
# add commit_version_output to macro
execute_process(
	COMMAND git describe --tags --always
	OUTPUT_VARIABLE CPPNET_VERSION
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions(-DCPPNET_VERSION="${CPPNET_VERSION}")
execute_process(
	COMMAND git log -1 --pretty=format:"%cd" --date=format:"%Y-%m-%d"
	OUTPUT_VARIABLE CPPNET_VERSION_DATE
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions(-DCPPNET_VERSION_DATE="${CPPNET_VERSION_DATE}")

# for check memory
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined -fno-omit-frame-pointer")
# link_libraries(-fsanitize=address -fsanitize=leak -fsanitize=undefined)

set(EXECUTABLE_OUTPUT_PATH ../../bin)

add_executable(test_bin ./main.cpp ${DIR_SRCS})

add_executable(link_bin ./main.cpp)
target_link_libraries(link_bin cppnet)
