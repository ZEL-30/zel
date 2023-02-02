add_executable(test_singleton test_singleton.cpp)
target_link_libraries(test_singleton
PUBLIC
    Utility
)


add_executable(utility_test utility_test.cpp)
target_link_libraries(utility_test
PUBLIC
    Utility
    doctest_with_main
)



