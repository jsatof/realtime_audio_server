set(audio_project_test_SOURCES
    test_main.cpp)

add_executable(audio_project_test ${audio_project_test_SOURCES})
target_include_directories(audio_project_test PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_directories(audio_project_test PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(audio_project_test PRIVATE audio_server_shared audio_client_shared fmt::fmt ${GTEST_LIBRARIES})
set_target_properties(audio_project_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)

add_custom_target(test COMMAND audio_project_test)
