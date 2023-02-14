# 最低版本
cmake_minimum_required(VERSION 3.24.2) 

# 设置项目名称
project(zel)

# 采用C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


# 添加子工程
add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(test)

# 打印
# MESSAGE( STATUS "目录 = ${CMAKE_CURRENT_SOURCE_DIR}/public_lib/lib/public.lib")