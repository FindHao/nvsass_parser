cmake_minimum_required(VERSION 3.12)
project(nvsass_parser)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
            "Default build type: RelWithDebInfo" FORCE)
endif ()

find_package(Protobuf REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/protoc)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/ nvsass_parser_src)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/protoc/ nvsass_parser_protoc)

file(GLOB nvsass_parser_lib_src CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/include/*.h" "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/protoc/*.cc" "${CMAKE_CURRENT_LIST_DIR}/protoc/*.h")
list(FILTER nvsass_parser_lib_src EXCLUDE REGEX "main.cpp")

add_executable(nvsass_parser_exe ${nvsass_parser_src} ${nvsass_parser_protoc})
target_link_libraries(nvsass_parser_exe PUBLIC
        -lprotobuf
        )
set_target_properties(nvsass_parser_exe PROPERTIES OUTPUT_NAME nvsass_parser)


add_library(nvsass_parser_lib SHARED ${nvsass_parser_lib_src})
target_link_libraries(nvsass_parser_lib PUBLIC
        -lprotobuf
        )
set_target_properties(nvsass_parser_lib PROPERTIES OUTPUT_NAME nvsass_parser)


