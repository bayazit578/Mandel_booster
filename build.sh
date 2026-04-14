#!/bin/bash

CFLAGS="-g -fno-omit-frame-pointer -Wall -Iinclude -lcsfml-graphics -lcsfml-window -lcsfml-system -mavx -mavx2 "
CFLAGS+=" $1 $2"

SOURCE="src/main.cpp src/draw.cpp src/calc.cpp"
COMPILE_OUT="prog"
COMPILE_DIR="compiled"

if [ $# -eq 3 ]; then
    CFLAGS+=" $3"
fi

mkdir -p $COMPILE_DIR

g++ -o $COMPILE_DIR/$COMPILE_OUT $SOURCE $CFLAGS
