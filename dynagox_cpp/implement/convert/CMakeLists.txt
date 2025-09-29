add_executable(convert_graph src/convert_graph.cpp)
target_link_libraries(convert_graph fmt::fmt)

add_executable(convert_lsqb src/convert_lsqb.cpp)
target_link_libraries(convert_lsqb fmt::fmt)

add_executable(convert_job src/convert_job.cpp)
target_link_libraries(convert_job fmt::fmt)

add_executable(convert_ceb src/convert_ceb.cpp)
target_link_libraries(convert_ceb fmt::fmt)

add_executable(convert_tpch src/convert_tpch.cpp)
target_link_libraries(convert_tpch fmt::fmt)

add_executable(convert_tpcds src/convert_tpcds.cpp)
target_link_libraries(convert_tpcds fmt::fmt)