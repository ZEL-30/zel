
add_executable(test_mysql test_mysql.cpp models/test_data.h)
target_include_directories(test_mysql 
PUBLIC 
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(test_mysql
PUBLIC
    zel
)

