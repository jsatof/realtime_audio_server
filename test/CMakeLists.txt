set(audio_server_TEST_SOURCES
    test_main.cpp)

add_executable(audio_server_test ${audio_server_TEST_SOURCES})
target_include_directories(audio_server_test PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_directories(audio_server_test PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(audio_server_test PRIVATE audio_server_shared fmt::fmt ${GTEST_LIBRARIES})
set_target_properties(audio_server_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)

add_custom_target(test COMMAND audio_server_test)
