#!/bin/bash

CFLAGS="-O2 -g -Wall -lcsfml-graphics -lcsfml-window -lcsfml-system -mavx -mavx2"

SOURCE="main.cpp draw.cpp calc.cpp"
OUTPUT="prog"
FILENAME="compiled"

mkdir -p $FILENAME
g++ -o $FILENAME/$OUTPUT $SOURCE $CFLAGS
./$FILENAME/$OUTPUT
