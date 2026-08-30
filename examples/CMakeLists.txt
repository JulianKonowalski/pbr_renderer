project("VQ_examples" LANGUAGES C CXX)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp)

#GLAD and GL linked only for early development purposes
target_link_libraries(${PROJECT_NAME} PRIVATE VQ OpenGL::GL glad glm)

target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_PATH="${CMAKE_SOURCE_DIR}/resources/")
