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
		set(OPENSSL_VERSION "")
		execute_process(
			COMMAND brew list --versions openssl
			OUTPUT_VARIABLE OPENSSL_VERSION_OUTPUT
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" OPENSSL_VERSION "${OPENSSL_VERSION_OUTPUT}")
		message(STATUS "OpenSSL version: ${OPENSSL_VERSION}")

		# for author local mac
		include_directories(/opt/homebrew/Cellar/openssl@3/${OPENSSL_VERSION}/include)
		link_directories(/opt/homebrew/Cellar/openssl@3/${OPENSSL_VERSION}/lib)
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

set(EXECUTABLE_OUTPUT_PATH ../../bin)

add_executable(test_bin ./main.cpp ${DIR_SRCS})

add_executable(link_bin ./main.cpp)
target_link_libraries(link_bin cppnet)
