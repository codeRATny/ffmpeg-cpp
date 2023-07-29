file(GLOB_RECURSE TEST_SRC_LIST
  ./*.cpp)

foreach(TEST_SRC ${TEST_SRC_LIST})
    get_filename_component(TEST_EXE_NAME ${TEST_SRC} NAME_WE)

    message(STATUS "add test: ${TEST_EXE_NAME}")
    add_executable(${TEST_EXE_NAME} ${TEST_SRC})
    target_compile_options(${TEST_EXE_NAME}
        PRIVATE ${COMPILE_OPTIONS_DEFAULT})

    target_compile_definitions(${TEST_EXE_NAME}
        PRIVATE ${MK_COMPILE_DEFINITIONS})

    if(USE_SOLUTION_FOLDERS)
        SET_PROPERTY(TARGET ${TEST_EXE_NAME} PROPERTY FOLDER "test")
    endif()

    target_link_libraries(${TEST_EXE_NAME} ffmpeg-cpp)
    target_include_directories(${TEST_EXE_NAME}
  PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_compile_options(${TEST_EXE_NAME} PRIVATE -fsanitize=leak)
    target_link_options(${TEST_EXE_NAME} PRIVATE -fsanitize=leak)
  
endforeach()