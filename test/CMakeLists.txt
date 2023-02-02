add_executable(test_singleton test_singleton.cpp)
target_link_libraries(test_singleton
PUBLIC
    Utility
)


add_executable(test_logger test_logger.cpp)
target_link_libraries(test_logger
PUBLIC
    Utility
)


