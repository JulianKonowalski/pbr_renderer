project("VQ_tests" LANGUAGES CXX)

enable_testing()

file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE
    GTest::gtest_main
    VQ
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})

add_custom_command(
    TARGET ${PROJECT_NAME}
    COMMENT "Run all tests"
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ./tests/${PROJECT_NAME}
)
