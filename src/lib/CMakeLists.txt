# cmake version
cmake_minimum_required(VERSION 3.16.3)
# project name
project(lib)

# set c++ version
set(CMAKE_CXX_STANDARD 17)

# include dir add,split by<space>
include_directories(../cppnet)

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
aux_source_directory(../cppnet/ssl DIR_SRCS)
aux_source_directory(../cppnet/socket DIR_SRCS)
aux_source_directory(../cppnet/log DIR_SRCS)

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

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_library(cppnet STATIC ${DIR_SRCS})
