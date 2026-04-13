#!/bin/bash

CFLAGS="-g -Wall -Iinclude -lcsfml-graphics -lcsfml-window -lcsfml-system \
        -mavx -mavx2 "
CFLAGS+=$1

SOURCE="src/main.cpp src/draw.cpp src/calc.cpp"
COMPILE_OUT="prog"
COMPILE_DIR="compiled"

if [ $# -eq 2 ]; then
    CFLAGS+=" $2 $3 -mavx -mavx2"
fi

mkdir -p $COMPILE_DIR

g++ -o $COMPILE_DIR/$COMPILE_OUT $SOURCE $CFLAGS
