add_library(hw1_lib printer.c)

target_include_directories(hw1_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(hw1_main main.c)
target_link_libraries(hw1_main PRIVATE hw1_lib)

if(BUILD_TESTS)
    add_executable(hw1_tests tests.c)
    target_link_libraries(hw1_tests PRIVATE hw1_lib)
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/input.csv
    ${CMAKE_CURRENT_BINARY_DIR}/input.csv
    COPYONLY
)
