cmake_minimum_required(VERSION 3.10)
project(Compiler)

set(outputdir "${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/Compiler/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/Compiler/src/*.h")

add_executable(ssc ${SOURCES} ${HEADERS})

set_target_properties(ssc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${outputdir}/Compiler"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${outputdir}/Compiler"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${outputdir}/Compiler"
)

target_include_directories(ssc PRIVATE
    "${CMAKE_SOURCE_DIR}/Compiler/src"
    "${CMAKE_SOURCE_DIR}/Compiler/src/Core"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(ssc PRIVATE COMPILE_DEBUG)
    target_compile_options(ssc PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/Od;/Zi>")
    target_link_options(ssc PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/DEBUG>")
    target_compile_options(ssc PRIVATE "$<$<CXX_COMPILER_ID:GNU>:-O0;-g>")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(ssc PRIVATE COMPILE_RELEASE)
    target_compile_options(ssc PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/O2>")
    target_compile_options(ssc PRIVATE "$<$<CXX_COMPILER_ID:GNU>:-O3>")
endif()