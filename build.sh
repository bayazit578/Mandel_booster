#!/bin/bash

CFLAGS="-O3 -g -Wall -lcsfml-graphics -lcsfml-window -lcsfml-system"

SOURCE="main.cpp draw.cpp calc.cpp"
OUTPUT="prog"
FILENAME="compiled"

mkdir -p $FILENAME
g++ -o $FILENAME/$OUTPUT $SOURCE $CFLAGS
./$FILENAME/$OUTPUT
