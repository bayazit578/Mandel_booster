#!/bin/bash

CFLAGS="-O2 -g -Wall -lcsfml-graphics -lcsfml-window -lcsfml-system -mavx -mavx2 "
SELECT_MODE=$1
CFLAGS+=$SELECT_MODE
BENCH_OUT=

if [ $# -eq 3 ]; then
    CFLAGS+=$2
    BENCH_OUT=$3
fi

SOURCE="main.cpp draw.cpp calc.cpp"
OUTPUT="prog"
FILENAME="compiled"

mkdir -p $FILENAME
g++ -o $FILENAME/$OUTPUT $BENCH_OUT $SOURCE $CFLAGS
./$FILENAME/$OUTPUT $BENCH_OUT
