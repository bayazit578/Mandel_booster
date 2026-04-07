#!/bin/bash

SOURCE="main.cpp draw.cpp calc.cpp"
OUTPUT="prog"
FILENAME="compiled"

mkdir -p $FILENAME
g++ -o $FILENAME/$OUTPUT $SOURCE -g -Wall -lcsfml-graphics -lcsfml-window -lcsfml-system
./$FILENAME/$OUTPUT
