add_library(dynagox_lib
        src/dynagox/dynagox_line5c.cpp
        src/dynagox/dynagox_line5s.cpp
        src/dynagox/dynagox_line5m.cpp
        src/dynagox/dynagox_line5cd.cpp
        src/dynagox/dynagox_line5gc.cpp
        src/dynagox/dynagox_dumbbell.cpp
        src/dynagox/dynagox_lsqb6.cpp
        src/dynagox/dynagox_job8.cpp
        src/dynagox/dynagox_job8s.cpp
        src/dynagox/dynagox_job8m1.cpp
        src/dynagox/dynagox_job8m2.cpp
        src/dynagox/dynagox_job16.cpp
        src/dynagox/dynagox_job19.cpp
        src/dynagox/dynagox_ceb1.cpp
        src/dynagox/dynagox_ceb2.cpp
        src/dynagox/dynagox_ceb3.cpp
        src/dynagox/dynagox_ceb4.cpp
        src/dynagox/dynagox_tpch3.cpp
        src/dynagox/dynagox_tpch5.cpp
        src/dynagox/dynagox_tpch8.cpp
        src/dynagox/dynagox_tpch9.cpp
        src/dynagox/dynagox_tpch10.cpp
        src/dynagox/dynagox_tpcds15.cpp
        src/dynagox/dynagox_tpcds19.cpp
        src/dynagox/dynagox_example.cpp
        src/dynagox/dynagox_line5s_d.cpp)
target_include_directories(dynagox_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/dynagox)
target_link_libraries(dynagox_lib
        multi_hash_map utils fmt::fmt)

add_executable(dynagox_exec src/dynagox/main.cpp)
target_link_libraries(dynagox_exec dynagox_lib)

add_library(crown_lib
        src/crown/crown_line5c.cpp
        src/crown/crown_line5s.cpp
        src/crown/crown_line5m.cpp
        src/crown/crown_line5cd.cpp
        src/crown/crown_line5gc.cpp
        src/crown/crown_dumbbell.cpp
        src/crown/crown_lsqb6.cpp
        src/crown/crown_job8.cpp
        src/crown/crown_job8s.cpp
        src/crown/crown_job8m1.cpp
        src/crown/crown_job8m2.cpp
        src/crown/crown_job16.cpp
        src/crown/crown_job19.cpp
        src/crown/crown_ceb1.cpp
        src/crown/crown_ceb2.cpp
        src/crown/crown_ceb3.cpp
        src/crown/crown_ceb4.cpp
        src/crown/crown_tpch3.cpp
        src/crown/crown_tpch5.cpp
        src/crown/crown_tpch8.cpp
        src/crown/crown_tpch9.cpp
        src/crown/crown_tpch10.cpp
        src/crown/crown_tpcds15.cpp
        src/crown/crown_tpcds19.cpp
        src/crown/crown_line5s_d.cpp)
target_include_directories(crown_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/crown)
target_link_libraries(crown_lib
        multi_hash_map utils fmt::fmt)

add_executable(crown_exec src/crown/main.cpp)
target_link_libraries(crown_exec crown_lib)