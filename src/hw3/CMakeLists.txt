add_library(hw3_lib states.c)

target_include_directories(hw3_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(hw3_main main.c)
target_link_libraries(hw3_main PRIVATE hw3_lib)

if(BUILD_TESTS)
    add_executable(hw3_tests tests.c)
    target_link_libraries(hw3_tests PRIVATE hw3_lib)
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/graph.txt
    ${CMAKE_CURRENT_BINARY_DIR}/graph.txt
    COPYONLY
)