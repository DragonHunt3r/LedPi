#!/bin/sh

if test "$#" -ne 1; then
    echo "Usage: <path-to-effect>" >&2
    exit 1
fi

cmake -D DEBUG=ON -D EFFECT:FILEPATH=$1 --log-level=VERBOSE -S . -B ./out
make -C ./out
