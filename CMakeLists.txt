cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)

project(gorilla-core)
set(LIBRARY_NAME ${CMAKE_STATIC_LIBRARY_PREFIX}${PROJECT_NAME})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_compile_definitions(PLATFORM_WINDOWS)
    set(PLATFORM_WINDOWS ON)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_compile_definitions(PLATFORM_LINUX)
    set(PLATFORM_LINUX ON)
elseif(UNIX AND APPLE)
    add_compile_definitions(PLATFORM_MAC)
    set(PLATFORM_MAC ON)
endif ()

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hash.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/singleton.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container/buffer_queue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container/command_queue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container/dictionary.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container/dictionary_data.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container/dictionary_node.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container/hash_map.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container/vector.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/file_system/file.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/file_system/file_system.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/file_system/path.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/log/log.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/memory/memory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/memory/memory_manager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/process/command_line_parser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/process/process.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/dictionary/dictionary_binary_stream.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/dictionary/dictionary_csv_stream.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/dictionary/dictionary_helper_stream.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/dictionary/dictionary_json_stream.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/stream_writer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/file_reader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/file_writer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/memory_reader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/memory_writer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/stream_reader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/stream_writer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/string/string.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/string/string16.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/string/string_helper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/thread/condition_variable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/thread/mutex.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/thread/thread.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/time/profiler.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/time/time.hpp
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container/buffer_queue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container/command_queue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container/dictionary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/container/dictionary_node.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/file_system/path.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/log/log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/memory/memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/memory/memory_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/process/command_line_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/dictionary/dictionary_binary_stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/dictionary/dictionary_csv_stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/dictionary/dictionary_helper_stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/dictionary/dictionary_json_stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/file_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/file_writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/memory_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stream/memory_writer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/string/string.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/string/string16.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/string/string_helper.cpp
)

if (PLATFORM_WINDOWS)
    set(SOURCES_PLATFORM
        ${CMAKE_CURRENT_SOURCE_DIR}/src/file_system/file_system_windows.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/file_system/file_windows.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/process/process_windows.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/thread/condition_variable_windows.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/thread/mutex_windows.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/thread/thread_windows.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/time/time_windows.cpp
    )
elseif (PLATFORM_MAC OR PLATFORM_LINUX)
    set(SOURCES_PLATFORM
        ${CMAKE_CURRENT_SOURCE_DIR}/src/file_system/file_system_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/file_system/file_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/process/process_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/thread/condition_variable_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/thread/mutex_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/thread/thread_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/time/time_unix.cpp
    )
    set(LIBRARIES_PLATFORM pthread)
endif()
set(SOURCES ${SOURCES} ${SOURCES_PLATFORM})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(${LIBRARY_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(PC_FILENAME lib${PROJECT_NAME}.pc)
set(PC_LINK_FLAGS -l${PROJECT_NAME})
configure_file(${PC_FILENAME}.in ${PC_FILENAME} @ONLY)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/
    DESTINATION include/${PROJECT_NAME}
    FILES_MATCHING
    PATTERN
    "*.hpp"
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PC_FILENAME} DESTINATION lib/pkgconfig)

enable_testing()
add_executable (unit-tests EXCLUDE_FROM_ALL tests/unit-tests.cpp)
target_include_directories(unit-tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(unit-tests ${LIBRARY_NAME} ${LIBRARIES_PLATFORM})
add_test(hash               unit-tests hash)
add_test(singleton          unit-tests singleton)
add_test(vector             unit-tests vector)
add_test(signal             unit-tests signal)
add_test(hash_map           unit-tests hash_map)
add_test(buffer_queue       unit-tests buffer_queue)
add_test(command_queue      unit-tests command_queue)
add_test(memory             unit-tests memory)
add_test(string             unit-tests string)
add_test(string16           unit-tests string16)
add_test(string_conversion  unit-tests string_conversion)
add_test(path               unit-tests path)
add_test(file_system        unit-tests file_system)
add_test(stream             unit-tests stream)
add_test(thread             unit-tests thread)
add_test(dictionary         unit-tests dictionary)
add_test(process            unit-tests process)
