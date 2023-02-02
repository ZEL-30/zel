# 添加已编译的库

# zel开发框架
add_library(Zel STATIC IMPORTED GLOBAL)
set_property(TARGET Zel PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/vendor/Zel/lib/zel.lib)
target_include_directories(Zel
INTERFACE
    ${CMAKE_SOURCE_DIR}/vendor/Zel/include
)

# winscard
add_library(Winscard STATIC IMPORTED GLOBAL)
set_property(TARGET Winscard PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/vendor/Winscard/lib/winscard.lib)
target_include_directories(Winscard
INTERFACE
    ${CMAKE_SOURCE_DIR}/vendor/Winscard/include
)