add_executable(test_singleton test_singleton.cpp)
target_link_libraries(test_singleton
PUBLIC
    zel
)


add_executable(test_utility test_utility.cpp)
target_include_directories(test_utility PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(test_utility
PUBLIC
    zel
)

add_executable(test_xml test_xml.cpp)
target_include_directories(test_xml PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(test_xml
PUBLIC
    zel
)

add_executable(test_json test_json.cpp)
target_include_directories(test_json PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(test_json
PUBLIC
    Json
)






add_executable(deep_copy deep_copy.cpp)
target_include_directories(deep_copy PUBLIC ${CMAKE_SOURCE_DIR}/vendor)


add_executable(test_interpreter test_interpreter.cpp)
target_include_directories(test_interpreter PUBLIC ${CMAKE_SOURCE_DIR}/vendor)
target_link_libraries(test_interpreter
PUBLIC
    Interpreter
)

find_package(fmt CONFIG REQUIRED)
add_executable(test_fmt test_fmt.cpp)
target_link_libraries(test_fmt 
PUBLIC 
    fmt::fmt
)

add_subdirectory(reflect)
add_subdirectory(mysql)
