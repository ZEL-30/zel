# 查找源文件
file(GLOB_RECURSE SRC_FILES  ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

# 查找头文件
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/*.h)

#  编译静态库
add_library(AstNode STATIC ${SRC_FILES} ${HEADER_FILES})

# 添加头文件
target_include_directories(AstNode
PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

# 为target添加需要链接的共享库
TARGET_LINK_LIBRARIES(AstNode
    Token
)
