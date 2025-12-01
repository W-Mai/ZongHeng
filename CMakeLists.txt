cmake_minimum_required(VERSION 3.10)
project(example)

set(CMAKE_CXX_STANDARD 17)

# set(DISABLE_ASAN true) # comment this line to enable ASAN(Address Sanitizer)

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

# Chainable API example
add_executable(chainable_example example/chainable_example.cpp)
target_link_libraries(chainable_example ZongHeng)

# test
enable_testing()

add_subdirectory(test)

add_test(
        NAME plus_test
        COMMAND $<TARGET_FILE:plus_test>
)

add_test(
        NAME plus_string_test
        COMMAND $<TARGET_FILE:plus_string_test>
)

add_test(
        NAME lian_test
        COMMAND $<TARGET_FILE:lian_test>
)

add_test(
        NAME yi_transform_test
        COMMAND $<TARGET_FILE:yi_transform_test>
)

add_test(
        NAME type_safety_test
        COMMAND $<TARGET_FILE:type_safety_test>
)

add_test(
        NAME operators_test
        COMMAND $<TARGET_FILE:operators_test>
)

add_test(
        NAME combinators_test
        COMMAND $<TARGET_FILE:combinators_test>
)
