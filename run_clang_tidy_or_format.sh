#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "usage: run_clang_tidy_or_format <CMAKE_BINARY_DIR> <PROJECT_ROOT_DIR>"
	exit
fi

CMAKE_BINARY_DIR=$1
PROJECT_ROOT_DIR=$2

function tidy(paths_list) {
	clang-tidy -p ${CMAKE_BINARY_DIR} --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy --use-color $paths_list
}

function format(paths_list) {
    clang-format --Werror --sort-includes --verbose $paths_list
}

function formatw(paths_list) {
    COMMAND clang-format --Werror --sort-includes --verbose -i $paths_lis${audio_server_SOURCES}t
}
