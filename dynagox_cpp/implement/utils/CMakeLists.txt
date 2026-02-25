add_library(utils src/utils.cpp)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(utils fmt::fmt)

if(WIN32)
    target_link_libraries(utils psapi)
endif()