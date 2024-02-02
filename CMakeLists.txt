cmake_minimum_required(VERSION 3.2)
project(ENginger)

set(PROJECT_NAME ENginger)

set(SOURCE_PATH src)
set(HEADER_PATH include)
set(THIRD_PARTY_PATH third-party)


set(GLFW_LIB_PATH "third-party/glfw/lib")

set(SOURCE_FILES "${SOURCE_PATH}/main.cpp" "${SOURCE_PATH}/shader-program.cpp" "${SOURCE_PATH}/glfw-utils.cpp" "${SOURCE_PATH}/renderer.cpp" "${THIRD_PARTY_PATH}/glad/glad.c" "${SOURCE_PATH}/filesystem-utils.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories (${PROJECT_NAME} PUBLIC ${HEADER_PATH})
target_include_directories (${PROJECT_NAME} PUBLIC ${THIRD_PARTY_PATH})

find_library(GLFW glfw3 "${GLFW_LIB_PATH}") # Папка Lib, где лежат файлы аналогичного расширения
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${GLFW}) # к результату find_library нужно обращаться через ${result}