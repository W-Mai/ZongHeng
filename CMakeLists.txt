cmake_minimum_required(VERSION 3.10)
project(example)

set(CMAKE_CXX_STANDARD 11)

set(DISABLE_ASAN true) # comment this line to enable ASAN(Address Sanitizer)

# Address Sanitizer
if (NOT DISABLE_ASAN)
    set(ASAN_FLAGS "-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
    add_definitions(${ASAN_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} ${ASAN_FLAGS})
endif ()

include_directories(source)
add_subdirectory(source)

file(GLOB APP_SOURCES
        example/example.cpp
        )

# Executable
add_executable(example ${APP_SOURCES})

# Link
target_link_libraries(example ZongHeng)

# test
enable_testing()

add_subdirectory(test)

add_test(
        NAME plus_test
        COMMAND $<TARGET_FILE:plus_test>
)
