#!/usr/bin/env bash

export SCRIPT_PATH=$(realpath $(dirname $BASH_SOURCE))
export COMPILER='gcc-7'; export UBUNTU='17.10'; sudo -s $SCRIPT_PATH/compile.sh