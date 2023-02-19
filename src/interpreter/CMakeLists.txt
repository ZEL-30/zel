# 查找源文件
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

# 查找头文件
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)

#  编译静态库
add_library(Interpreter "")

target_sources(Interpreter
PRIVATE
    ${SOURCES}
PUBLIC
    ${HEADERS}
)

# 添加头文件
target_include_directories(Interpreter
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

# 为target添加需要链接的共享库
TARGET_LINK_LIBRARIES(Interpreter
PUBLIC
)