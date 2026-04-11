#!/bin/bash

CFLAGS="-O2 -g -Wall -Iinclude -lcsfml-graphics -lcsfml-window -lcsfml-system -mavx -mavx2 "
SELECT_MODE=$1
CFLAGS+=$SELECT_MODE
BENCH_OUT=

if [ $# -eq 3 ]; then
    CFLAGS+=" $2"
    BENCH_OUT=$3
    touch $BENCH_OUT
fi

SOURCE="src/main.cpp src/draw.cpp src/calc.cpp"
OUTPUT="prog"
DIR="compiled"

mkdir -p $DIR

g++ -o $DIR/$OUTPUT $SOURCE $CFLAGS
./$DIR/$OUTPUT $BENCH_OUT
