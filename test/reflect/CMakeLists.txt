file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)
add_executable(test_reflect ${SOURCES} ${HEADERS})
target_link_libraries(test_reflect
PUBLIC
    Reflect
)

