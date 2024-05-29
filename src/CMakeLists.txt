set(audio_server_SOURCES
    stub.cpp
    server/server.cpp
)

add_executable(audio_server server/main.cpp ${audio_server_SOURCES})
target_include_directories(audio_server PRIVATE
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/src/util
	${PROJECT_SOURCE_DIR}/src/server)
target_link_directories(audio_server PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(audio_server PRIVATE fmt::fmt)

set(audio_client_SOURCES
	stub.cpp
)

add_executable(audio_client client/main.cpp ${audio_client_SOURCES})
target_include_directories(audio_client PRIVATE
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/src/util
	${PROJECT_SOURCE_DIR}/src/client)
target_link_directories(audio_client PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(audio_client PRIVATE fmt::fmt ${JUCE_LIBRARIES})

if (UNIX)
    target_link_libraries(audio_server PRIVATE dl pthread m)
    target_link_libraries(audio_client PRIVATE dl pthread m)
endif()

install(TARGETS audio_server RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(TARGETS audio_client RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if (BUILD_TESTS)
    add_library(audio_server_shared SHARED ${audio_server_SOURCES})
    target_include_directories(audio_server_shared PRIVATE ${PROJECT_SOURCE_DIR}/src)
    target_link_directories(audio_server_shared PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    target_link_libraries(audio_server_shared PRIVATE fmt::fmt)
    install(TARGETS audio_server_shared LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

    add_library(audio_client_shared SHARED ${audio_server_SOURCES})
    target_include_directories(audio_client_shared PRIVATE ${PROJECT_SOURCE_DIR}/src)
    target_link_directories(audio_client_shared PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    target_link_libraries(audio_client_shared PRIVATE fmt::fmt ${JUCE_LIBRARIES})
    install(TARGETS audio_client_shared LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endif()

# Prepend the full path to each source file
# Create custom target for running clang tidy
#list(TRANSFORM audio_server_SOURCES REPLACE ${CMAKE_SOURCE_DIR}/src/)

set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS}
    -extra-arg=-Wno-unknown-warning-option
    -extra-arg=-Wno-ignored-optimization-argument
    -extra-arg=-Wno-unused-command-line-argument
    -warning-as-errors=*)

if("${CLANG_TIDY_OPTIONS_DRIVER_MODE}" STREQUAL "cl")
    set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=/std:c++${CMAKE_CXX_STANDARD})
else()
    set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=-std=c++${CMAKE_CXX_STANDARD})
endif()

add_custom_target(tidy
    COMMAND ${PROJECT_ROOT_DIR}/run_clang_tidy_or_format ${CMAKE_BINARY_DIR} ${PROJECT_SOURCE_DIR}
    COMMENT "Running tidy checks"
    VERBATIM)

add_custom_target(format
    COMMAND ${PROJECT_ROOT_DIR}/run_clang_tidy_or_format ${CMAKE_BINARY_DIR} ${PROJECT_SOURCE_DIR}
    COMMENT "Running formatting checks"
    VERBATIM)

add_custom_target(formatw
    COMMAND ${PROJECT_ROOT_DIR}/run_clang_tidy_or_format ${CMAKE_BINARY_DIR} ${PROJECT_SOURCE_DIR}
	COMMENT "Running formatting checks"
    VERBATIM)
