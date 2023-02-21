add_executable(singleton_test singleton_test.cpp)
target_link_libraries(singleton_test
PUBLIC
    zel
)


add_executable(utility_test utility_test.cpp)
target_include_directories(utility_test PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(utility_test
PUBLIC
    zel
)

add_executable(xml_test xml_test.cpp)
target_include_directories(xml_test PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(xml_test
PUBLIC
    zel
)

add_executable(json_test json_test.cpp)
target_include_directories(json_test PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(json_test
PUBLIC
    Json
)



add_executable(mysql_test mysql_test.cpp models/test_data.h)
target_include_directories(mysql_test 
PUBLIC 
    ${CMAKE_SOURCE_DIR}/vendor
)
target_link_libraries(mysql_test
PUBLIC
    zel
)


add_executable(deep_copy deep_copy.cpp models/test_data.h)
target_include_directories(deep_copy PUBLIC ${CMAKE_SOURCE_DIR}/vendor)


add_executable(interpreter_test interpreter_test.cpp models/test_data.h)
target_include_directories(interpreter_test PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(interpreter_test
PUBLIC
    Script
)


