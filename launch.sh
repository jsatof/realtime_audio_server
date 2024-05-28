#!/bin/bash

PORT=34345
build/bin/audio_server $PORT &
build/bin/audio_client $PORT
#build/bin/audio_client kill
