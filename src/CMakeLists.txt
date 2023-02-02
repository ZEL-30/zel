# 生成可执行文件
ADD_EXECUTABLE(run main.cpp)


add_subdirectory(Utility)


# 添加头文件
target_link_directories(run
PUBLIC
	${CMAKE_CURRENT_LIST_DIR}
)

# 为target添加需要链接的共享库
TARGET_LINK_LIBRARIES(run
PRIVATE
)
