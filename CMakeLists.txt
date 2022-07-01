cmake_minimum_required(VERSION 3.12)
project(nvsass_parser)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
            "Default build type: RelWithDebInfo" FORCE)
endif()

find_package(Protobuf REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/src
                    ${CMAKE_CURRENT_SOURCE_DIR}/protoc)

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src/  nvsass_parser_src)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/protoc/ nvsass_parser_protoc)


add_executable(nvsass_parser ${nvsass_parser_src} ${nvsass_parser_protoc})

target_link_libraries(nvsass_parser
        -lprotobuf
)


