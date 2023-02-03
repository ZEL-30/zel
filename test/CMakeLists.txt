add_executable(singleton_test singleton_test.cpp)
target_include_directories(singleton_test PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(singleton_test
PUBLIC
    Utility
)


add_executable(utility_test utility_test.cpp)
target_include_directories(utility_test PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(utility_test
PUBLIC
    Utility
)

add_executable(database_test database_test.cpp)
target_include_directories(database_test PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(database_test
PUBLIC
    DataBase
)
