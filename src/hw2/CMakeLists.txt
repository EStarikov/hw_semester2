add_library(hw2_lib registration.c)

target_include_directories(hw2_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(hw2_utility utility.c)
target_link_libraries(hw2_utility PRIVATE hw2_lib)

if(BUILD_TESTS)
    add_executable(hw2_tests tests.c)
    target_link_libraries(hw2_tests PRIVATE hw2_lib)
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/airports.txt
    ${CMAKE_CURRENT_BINARY_DIR}/airports.txt
    COPYONLY
)
