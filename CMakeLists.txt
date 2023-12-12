cmake_minimum_required(VERSION 3.2)
project(ENginger)

set(PROJECT_NAME ENginger)
set(SOURCE_PATH src)
set(SOURCE_FILES "${SOURCE_PATH}/main.cpp" "${SOURCE_PATH}/shader-program.cpp" "${SOURCE_PATH}/glfw-utils.cpp" "${SOURCE_PATH}/renderer.cpp" "${SOURCE_PATH}/glad.c" "${SOURCE_PATH}/path.cpp" "${SOURCE_PATH}/config-data.cpp")
set(HEADER_PATH "${SOURCE_PATH}/include")
set(LIBRARY_PATH "${SOURCE_PATH}/lib")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories (${PROJECT_NAME} PUBLIC ${HEADER_PATH})

find_library(GLFW glfw3 "${LIBRARY_PATH}") # Папка Lib, где лежат файлы аналогичного расширения
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${GLFW}) # к результату find_library нужно обращаться через ${result}