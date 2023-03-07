add_executable(test_serialize test_serialize.cpp )
target_include_directories(test_serialize PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(test_serialize
PUBLIC
    Serialize
)

