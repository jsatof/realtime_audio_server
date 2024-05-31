set(audio_server_SOURCES
	server/main.cpp)

set(audio_client_SOURCES
	client/main.cpp
	ring_buffer.cpp)

add_executable(realtime_audio_mixer main.cpp ${audio_server_SOURCES} ${audio_client_SOURCES})
target_include_directories(realtime_audio_mixer PRIVATE
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/src/util
	${OPUS_INCLUDE_DIRS})
target_link_directories(realtime_audio_mixer PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(realtime_audio_mixer PRIVATE fmt::fmt ${SDL2_LIBRARIES} ${OPUS_LIBRARIES} ${OPUSFILE_LIBRARIES})

if (UNIX)
	target_link_libraries(realtime_audio_mixer PRIVATE dl pthread m)
endif()

install(TARGETS realtime_audio_mixer RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if (BUILD_TESTS)
	add_library(audio_server_shared SHARED ${audio_server_SOURCES})
	target_include_directories(audio_server_shared PRIVATE
		${PROJECT_SOURCE_DIR}/src
		${PROJECT_SOURCE_DIR}/src/util
		${OPUS_INCLUDE_DIRS})
	target_link_directories(audio_server_shared PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
	target_link_libraries(audio_server_shared PRIVATE fmt::fmt ${OPUS_LIBRARIES} ${OPUSFILE_LIBRARIES})
	install(TARGETS audio_server_shared LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

	add_library(audio_client_shared SHARED ${audio_client_SOURCES})
	target_include_directories(audio_client_shared PRIVATE
		${PROJECT_SOURCE_DIR}/src
		${PROJECT_SOURCE_DIR}/src/util
		${OPUS_INCLUDE_DIRS})
	target_link_directories(audio_client_shared PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
	target_link_libraries(audio_client_shared PRIVATE fmt::fmt ${SDL2_LIBRARIES} ${OPUS_LIBRARIES} ${OPUSFILE_LIBRARIES})
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
