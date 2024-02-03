cmake_minimum_required(VERSION 3.2)
project(ENginger)

set(PROJECT_NAME ENginger)

set(SOURCE_PATH src)
set(HEADER_PATH include)
set(THIRD_PARTY_PATH third-party)
set(GLFW_PATH "third-party/glfw")

add_subdirectory(${GLFW_PATH})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(SOURCE_FILES
        "${SOURCE_PATH}/main.cpp"
        "${SOURCE_PATH}/shader-program.cpp"
        "${SOURCE_PATH}/glfw-utils.cpp"
        "${SOURCE_PATH}/renderer.cpp"
        "${SOURCE_PATH}/filesystem-utils.cpp"
        "${THIRD_PARTY_PATH}/glad/glad.c"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories (${PROJECT_NAME} PUBLIC ${HEADER_PATH})
target_include_directories (${PROJECT_NAME} PUBLIC ${THIRD_PARTY_PATH})
target_include_directories (${PROJECT_NAME} PUBLIC "${GLFW_PATH}/include")

# find_library(GLFW glfw3 "${GLFW_LIB_PATH}") # Папка Lib, где лежат файлы аналогичного расширения
target_link_libraries(${PROJECT_NAME} glfw) # к результату find_library нужно обращаться через ${result}