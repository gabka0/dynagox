function(clang_format TARGET_NAME SOURCE_DIR)
    find_program(CLANG-FORMAT_PATH clang-format REQUIRED)
    file(GLOB_RECURSE FORMAT_SOURCES LIST_DIRECTORIES false
            "${SOURCE_DIR}/*.cpp"
            "${SOURCE_DIR}/*.h")

    add_custom_target(${TARGET_NAME}
            COMMAND ${CLANG-FORMAT_PATH} -style=llvm -i ${FORMAT_SOURCES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endfunction()