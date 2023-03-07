add_executable(test_reflect test_reflect.cpp student.hpp string.hpp)
target_include_directories(test_reflect PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(test_reflect
PUBLIC
    Reflect
)

add_executable(offsetof offsetof.cpp)

add_executable(func_ptr func_ptr.cpp)