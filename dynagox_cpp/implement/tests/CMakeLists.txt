include(Catch2)

add_executable(tests
        multi_hash_map/memory_pool_test.cpp
        multi_hash_map/multi_hash_map_test.cpp
        multi_hash_map/primary_hash_index_test.cpp
        multi_hash_map/secondary_hash_index_test.cpp
        multi_hash_map/singleton_test.cpp
        utils/utils_test.cpp)

target_link_libraries(tests PRIVATE
        multi_hash_map
        utils
        Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)