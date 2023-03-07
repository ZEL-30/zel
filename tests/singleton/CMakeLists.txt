add_executable(test_singleton test_singleton.cpp test.hpp)
target_include_directories(test_singleton PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(test_singleton
PUBLIC
    Utility
)

